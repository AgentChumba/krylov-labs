#include "../include/Tab2Widget.h"
#include "../include/OscB322Lib_TLB.h"

#ifdef Q_OS_WIN

#include <QDebug>

class CSink : public QObject, public Oscb322lib_tlb::_IOscEvents {
    Q_OBJECT

public:
    explicit CSink(QObject *parent = nullptr);
    virtual ~CSink();

    HRESULT STDMETHODCALLTYPE OnDataReady(unsigned long nChannelsMask) override;
    STDMETHOD(QueryInterface)(REFIID iid, void **ppvObject) override;
    ULONG STDMETHODCALLTYPE AddRef() override;
    ULONG STDMETHODCALLTYPE Release() override;

protected:
    std::atomic<ULONG> m_dwRefCount;
    void OnGotData(unsigned long nChannelsMask);
};

Oscb322lib_tlb::IOsc            *m_pDevice;
Oscb322lib_tlb::IOscChannel     *m_pChanA;
CSink               *m_pSink;
IConnectionPoint    *m_pCP;
DWORD               m_dwAdvise;

double* VoltsTbl = NULL;
int VoltsTblSize = 0;

int const FreqTblSize = 15;
double FreqTbl[FreqTblSize];
bool IsDataReady = false;

bool m_bActivateA = true;
UINT m_GainA = 6;
double SelectedVoltsRange;

int Synhro = 1;
double SynhroLevel = 0;
double ZeroLevel = 0.0;

UINT m_SamplingFreq = 14;
double SelectedFreqRange;

ULONG MaxDataLength = 4096;
UINT  m_DataSize = 4096;

UINT iAnimateStep = 5;
long int aTime = 0;

CSink::CSink(QObject *parent)
    : QObject(parent), m_dwRefCount(0) {}

CSink::~CSink() {}

HRESULT CSink::OnDataReady(unsigned long nChannelsMask) {
    OnGotData(nChannelsMask);
    return S_OK;
}

STDMETHODIMP CSink::QueryInterface(REFIID iid, void **ppvObject) {
    if (iid == Oscb322lib_tlb::IID__IOscEvents || iid == IID_IUnknown) {
        AddRef();
        *ppvObject = static_cast<void *>(this);
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG CSink::AddRef() {
    return m_dwRefCount.fetch_add(1, std::memory_order_relaxed) + 1;
}

ULONG CSink::Release() {
    ULONG count = m_dwRefCount.fetch_sub(1, std::memory_order_relaxed) - 1;
    if (count == 0) delete this;
    return count;
}

void CSink::OnGotData(unsigned long nChannelsMask)
{
    IsDataReady = true;
}

#else
#include <QLabel>
#include <QVBoxLayout>
#endif

Tab2Widget::~Tab2Widget() {}

Tab2Widget::Tab2Widget(QWidget *parent)
    : QWidget(parent) {

#ifdef Q_OS_WIN
    QLabel *labelSamplingRate = new QLabel("Частота дискретизации:");
    comboSamplingRate = new QComboBox();

    btnInitCom = new QPushButton("Инициализация COM подсистемы");
    btnInitDevice = new QPushButton("Инициализировать устройство");
    btnFetchData = new QPushButton("Получить данные");
    btnStop = new QPushButton("Стоп");

    plot = new QCustomPlot(this);
    plot->addGraph();
    plot->xAxis->setLabel("Время");
    plot->yAxis->setLabel("Амплитуда");
    plot->setMinimumSize(400, 400);

    QHBoxLayout *rateLayout = new QHBoxLayout();
    rateLayout->addWidget(labelSamplingRate);
    rateLayout->addWidget(comboSamplingRate);
    rateLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(btnInitCom);
    buttonLayout->addWidget(btnInitDevice);
    buttonLayout->addWidget(btnFetchData);
    buttonLayout->addWidget(btnStop);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(rateLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(plot);
    mainLayout->addStretch();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Tab2Widget::onTimeout);

    connect(comboSamplingRate, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboSamplingRateChanged()));

    connect(btnInitCom, &QPushButton::clicked, this, &Tab2Widget::initComSubsystem);
    connect(btnInitDevice, &QPushButton::clicked, this, &Tab2Widget::initDevice);
    connect(btnFetchData, &QPushButton::clicked, this, &Tab2Widget::fetchData);
    connect(btnStop, &QPushButton::clicked, this, &Tab2Widget::stopFetching);
#else
    QLabel *title = new QLabel("Данная OS не поддерживается подключаемым ПО");
    title->setStyleSheet("font-size: 24px; font-weight: bold; text-align: center;");
    title->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
#endif
}

#ifdef Q_OS_WIN

#include <string>
#include <cstdio>

void FreqToStringMark(double freq, std::string& FreqMark) {
    int value;
    if (freq >= 1e9) {
        value = (int)(freq / 1e9);
        FreqMark = std::to_string(value) + " GHz";
    } else if (freq >= 1e6) {
        value = (int)(freq / 1e6);
        FreqMark = std::to_string(value) + " MHz";
    } else if (freq >= 1e3) {
        value = (int)(freq / 1e3);
        FreqMark = std::to_string(value) + " KHz";
    } else {
        value = (int)freq;
        FreqMark = std::to_string(value) + " Hz";
    }
}

void Tab2Widget::initComSubsystem() {
    if (!m_pDevice) {
        CoUninitialize();
        // CoUninitialize(); // в оригинале 3 раза - хз зачем
        // CoUninitialize();

        HRESULT result = CoInitialize(NULL);
        if (FAILED(result)) {
            QMessageBox::critical(this, "Error", "Failed to initialize COM library!");
            return;
        }

        result = CoCreateInstance(Oscb322lib_tlb::CLSID_OscDeviceB322, NULL, CLSCTX_ALL, Oscb322lib_tlb::IID_IOsc, reinterpret_cast<void**>(&m_pDevice));
        if (FAILED(result)) {
            QMessageBox::critical(this, "Error",
                "Failed to create COM instance for Oscilloscope.\n"
                "Make sure the Oscilloscope driver is installed.");
            m_pDevice = nullptr;
            return;
        }
    }
}

void Tab2Widget::initDevice() {
    ULONG nChanNum = 0;

    HRESULT result = m_pDevice->get_ChannelsCount(&nChanNum);
    if (FAILED(result)) {
        QMessageBox::critical(this, "Error", "Failed to get channels count.");
        return;
    }

    if (nChanNum > 0) {
        result = m_pDevice->get_Channel(0, &m_pChanA);
        if (FAILED(result)) {
            QMessageBox::critical(this, "Error", "Failed to get channel A.");
            return;
        }
    }

    ULONG timebasemax;
    result = m_pChanA->get_TimeBaseBounds(1, &timebasemax);
    if (FAILED(result)) {
        QMessageBox::critical(this, "Error", "Failed to get time base bounds.");
        return;
    }

    double freq;
    std::string FreqMark;
    UINT res;

    for (int i = 0; i < FreqTblSize; i++) {
        res = m_pChanA->get_TimeBaseToFreq(i, &freq);
        // result = m_pChanA->get_TimeBaseToFreq(i, &freq); // На слyчай ошибки предыдущего
                                                            // но как править потом дальше - хз
        if (FAILED(res)) {  // Может надо ! всунуть, делать по обстоятелствам (или вообще снести проверку)
            QMessageBox::critical(this, "Error", "Failed to get time base frequency.");
            return;
        }
        FreqTbl[i] = freq;

        FreqToStringMark(freq, FreqMark);
        comboSamplingRate->addItem(QString::fromStdString(FreqMark));
    }

    comboSamplingRate->setCurrentIndex(0);

    res = m_pChanA->set_TimeBase(comboSamplingRate->currentIndex());
    if (FAILED(res)) {
        QMessageBox::critical(this, "Error", "Failed to set time base.");
        return;
    }

    int ir = m_pChanA->get_DataSizeBounds(1, &MaxDataLength);
    if (FAILED(ir)) {
        QMessageBox::critical(this, "Error", "Failed to get data size bounds.");
        return;
    }
    m_pChanA->set_DataSize(m_DataSize);

    IConnectionPointContainer* pCPC = nullptr;
    result = m_pDevice->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
    if (FAILED(result)) {
        QMessageBox::critical(this, "Error", "Failed to query connection point container.");
        return;
    }

    result = pCPC->FindConnectionPoint(Oscb322lib_tlb::IID__IOscEvents, &m_pCP);
    if (FAILED(result)) {
        QMessageBox::critical(this, "Error", "Failed to find connection point.");
        pCPC->Release();
        return;
    }
    pCPC->Release(); // Тут не ошибка Игорь - она должна быть и там, и там

    m_pSink = new CSink();
    m_pSink->AddRef();
    result = m_pCP->Advise(m_pSink, &m_dwAdvise);
    if (FAILED(result)) {
        QMessageBox::critical(this, "Error", "Failed to advise sink.");
        return;
    }
}

void Tab2Widget::ShowImage() {
    double dt = 1.0 / FreqTbl[comboSamplingRate->currentIndex()];
    
    plot->clearGraphs();
    plot->addGraph();

    for (int i = 0; i < data.size(); i++) {
        double xValue = (aTime + i) * dt;
        double yValue = data[i];
        plot->graph(0)->addData(xValue, yValue);
    }
    plot->replot();
}

void Tab2Widget::fetchData() {
    btnFetchData->setEnabled(false);
    btnStop->setEnabled(true);

    m_pDevice->set_Run(true);
    timer->start(100);
}

void Tab2Widget::stopFetching() {
    btnFetchData->setEnabled(true);
    btnStop->setEnabled(false);

    m_pDevice->set_Run(false);
    timer->stop();
}

void Tab2Widget::onTimeout() {
    ULONG nRealDataCnt;
    std::vector<double> tmp(m_DataSize + 1, 0.0);

    if (IsDataReady) {
        data.clear();
        timer->stop();
        m_pDevice->set_Run(false);

        HRESULT hr0 = m_pChanA->GetSignalData(m_DataSize, &tmp[0]);
        nRealDataCnt = hr0 & 0x7fffffff;
        IsDataReady = false;

        for (size_t i = 0; i < nRealDataCnt; i++) data.push_back(tmp[i]);
        ShowImage();

        // Comment By GPT (Так-то это реально закоменченный Крылоым код)
        // Можно сюда добавить логику для удаления старых данных или анимации (если нужно)
        // Например, сдвиг временной оси:
        // for (int i = 0; i < iAnimateStep; i++) {
        //     data.erase(data.begin());
        //     aTime++;
        // }
    }
    timer->start(100);
}

void Tab2Widget::onComboSamplingRateChanged() {
    m_pChanA->set_TimeBase(comboSamplingRate->currentIndex());
}

#endif
