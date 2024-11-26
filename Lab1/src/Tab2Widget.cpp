#include "../include/Tab2Widget.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "setupapi.lib")
#endif

Tab2Widget::~Tab2Widget() {}

Tab2Widget::Tab2Widget(QWidget *parent)
    : QWidget(parent),
      titleLabel(new QLabel("Поиск USB устройств", this)),
      startButton(new QPushButton("Старт", this)),
      mouseList(new QListWidget(this)),
      keyboardList(new QListWidget(this)),
      hidList(new QListWidget(this)) {

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(startButton);
    leftLayout->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(new QLabel("Мышь", this));
    rightLayout->addWidget(mouseList);
    rightLayout->addWidget(new QLabel("Клавиатура", this));
    rightLayout->addWidget(keyboardList);
    rightLayout->addWidget(new QLabel("HID устройства", this));
    rightLayout->addWidget(hidList);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 3);

    setLayout(mainLayout);

    connect(startButton, &QPushButton::clicked, this, &Tab2Widget::onStartClicked);
}

void Tab2Widget::onStartClicked() {
    mouseList->clear();
    keyboardList->clear();
    hidList->clear();

    populateUsbDevices();
}

void Tab2Widget::populateUsbDevices() {

#ifdef Q_OS_LINUX
    libusb_context *ctx = nullptr;
    libusb_device **deviceList = nullptr;
    ssize_t deviceCount;

    if (libusb_init(&ctx) < 0) {
        qDebug("Ошибка инициализации libusb!");
        return;
    }

    deviceCount = libusb_get_device_list(ctx, &deviceList);
    if (deviceCount < 0) {
        qDebug("Ошибка получения списка USB-устройств!");
        libusb_exit(ctx);
        return;
    }
    
    QSet<QString> processedDevices;

    for (ssize_t i = 0; i < deviceCount; ++i) {
        libusb_device *device = deviceList[i];
        libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(device, &desc) == 0) {
            QString vendorId = QString::number(desc.idVendor, 16).toUpper();
            QString productId = QString::number(desc.idProduct, 16).toUpper();

            QString manufacturer = "Unknown";
            QString productName = "Unknown";
            QString serialNumber = "Unknown";

            libusb_device_handle *handle  = nullptr;
            if (libusb_open(device, &handle) == 0) {
                unsigned char buffer[256];
                if (desc.iManufacturer &&
                    libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, buffer, sizeof(buffer)) > 0) {
                    manufacturer = QString::fromUtf8((char *)buffer);
                } else {
                    qDebug("Ошибка чтения, не удалось получить данные.");
                }
                if (desc.iProduct &&
                    libusb_get_string_descriptor_ascii(handle, desc.iProduct, buffer, sizeof(buffer)) > 0) {
                    productName = QString::fromUtf8((char *)buffer);
                } else {
                    qDebug("Ошибка чтения, не удалось получить данные.");
                }
                if (desc.iSerialNumber &&
                    libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, buffer, sizeof(buffer)) > 0) {
                    serialNumber = QString::fromUtf8((char *)buffer);
                } else {
                    qDebug("Ошибка чтения, не удалось получить данные.");
                }
                libusb_close(handle);
            } else {
                qDebug("Не удалось открыть устройство для чтения строковых дескрипторов.");
            }

            QString detailedInfo = QString("VID: %1, PID: %2, %3 %4 %5")
                                       .arg(vendorId)
                                       .arg(productId)
                                       .arg(manufacturer)
                                       .arg(productName)
                                       .arg(serialNumber);

            switch (desc.bDeviceClass) {
            case LIBUSB_CLASS_HID:
                hidList->addItem(detailedInfo + " (HID Class)");
                break;
            case LIBUSB_CLASS_VIDEO:
                mouseList->addItem(detailedInfo);
                break;
            case LIBUSB_CLASS_DATA:
                keyboardList->addItem(detailedInfo);
                break;
            default:
                hidList->addItem(detailedInfo + " (Unknown Class)");
                break;
            }
        }
    }

    libusb_free_device_list(deviceList, 1);
    libusb_exit(ctx);
#endif

#ifdef Q_OS_WIN
        // Получаем список всех USB-устройств
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, NULL, NULL, DIGCF_PRESENT);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        qDebug() << "Не удалось получить список устройств.";
        return;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;

    // Перебираем устройства
    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex++, &deviceInfoData)) {
        // Получаем описание устройства
        TCHAR deviceName[1024];
        DWORD requiredSize = 0;
        if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, 
                                             (PBYTE)deviceName, sizeof(deviceName), &requiredSize)) {
            QString deviceNameStr = QString::fromWCharArray(deviceName);

            // Проверяем, является ли устройство мышью, клавиатурой или HID устройством
            if (deviceNameStr.contains("Mouse", Qt::CaseInsensitive)) {
                mouseList->addItem(deviceNameStr);
            } else if (deviceNameStr.contains("Keyboard", Qt::CaseInsensitive)) {
                keyboardList->addItem(deviceNameStr);
            } else if (deviceNameStr.contains("HID", Qt::CaseInsensitive)) {
                hidList->addItem(deviceNameStr);
            }
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
#endif
}
