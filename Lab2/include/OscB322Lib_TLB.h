#ifndef   OscB322Lib_TLBH
#define   OscB322Lib_TLBH

#include <olectl.h>
#include <ocidl.h>
#include <comdef.h>
#include <unknwn.h>
#include <atlcomcli.h>

namespace Oscb322lib_tlb {

extern const GUID LIBID_OscB322Lib;
extern const GUID IID_IOscChannel;
extern const GUID CLSID_OscChannelB322;
extern const GUID IID_IVgaConfig;
extern const GUID IID_IOsc;
extern const GUID IID_IOsc2;
extern const GUID IID_IOscCalibration;
extern const GUID IID_ISpecific;
extern const GUID IID__IOscEvents;
extern const GUID GUID_enmOscMode;
extern const GUID IID_IDataReadyF;
extern const GUID IID_IStateRunEvent;
extern const GUID IID_IStateStopEvent;
extern const GUID IID_IStateErrorStopEvent;
extern const GUID IID_IStateLooseConnectionEvent;
extern const GUID IID_IStateRestoreConnectionEvent;
extern const GUID CLSID_OscDeviceB322;
extern const GUID CLSID_OscDeviceB322Im;
extern const GUID CLSID_ZeroLevClbr;
extern const GUID CLSID_ZeroLevPPage;
extern const GUID IID_IOscCalibrationRIS;
extern const GUID CLSID_RisClbr;
extern const GUID CLSID_RisClbrPPage;
extern const GUID CLSID_SyncClbr;
extern const GUID CLSID_TrigCalibration;
extern const GUID IID_IPropertyPage;
extern const GUID CLSID_TrigPPage;
extern const GUID IID_IPropertyPageSite;
extern const GUID GUID_wireHWND;
extern const GUID GUID_UINT_PTR;
extern const GUID GUID___MIDL_IWinTypes_0009;
extern const GUID GUID__RemotableHandle;
extern const GUID GUID_LONG_PTR;
extern const GUID GUID_tagPOINT;
extern const GUID GUID_tagMSG;
extern const GUID GUID_tagRECT;
extern const GUID GUID_tagSIZE;
extern const GUID GUID_tagPROPPAGEINFO;
extern const GUID IID_IDAC;
extern const GUID CLSID_dac;
extern const GUID CLSID_DAC_PPage;

enum class enmOscMode
{
    Oscilloscope = 0,
    Segmented = 1,
    RIS = 2,
    SegmentedRIS = 3,
    Logger = 4
};

union  __MIDL_IWinTypes_0009
{
    long hInproc;
    long hRemote;
};

struct _RemotableHandle
{
    long fContext;
    Oscb322lib_tlb::__MIDL_IWinTypes_0009 u;
};

typedef Oscb322lib_tlb::_RemotableHandle* wireHWND;
typedef unsigned long UINT_PTR;
typedef long LONG_PTR;

struct tagPOINT
{
    long x;
    long y;
};

struct tagMSG
{
    Oscb322lib_tlb::wireHWND hwnd;
    unsigned message;
    Oscb322lib_tlb::UINT_PTR wParam;
    Oscb322lib_tlb::LONG_PTR lParam;
    unsigned long time;
    Oscb322lib_tlb::tagPOINT pt;
};

struct tagRECT
{
    long left;
    long top;
    long right;
    long bottom;
};

interface IOscChannel;
typedef CComPtr<IOscChannel> IOscChannelPtr;
// typedef IOscChannel* IOscChannelPtr;         Или так...

interface IVgaConfig;
typedef CComPtr<IVgaConfig> IVgaConfigPtr;

interface IOsc;
typedef CComPtr<IOsc> IOscPtr;

interface IOsc2 : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_RuningState(long* pbRuning) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IsModeSupported(Oscb322lib_tlb::enmOscMode eMode,
                                                          long* pbSupported) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Mode(Oscb322lib_tlb::enmOscMode* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Mode(Oscb322lib_tlb::enmOscMode pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MinLoggerPacketSize(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_MinLoggerPacketSize(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MinLoggerDelayMs(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_MinLoggerDelayMs(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_SegmentsCnt(unsigned piSegments) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SegmentsCnt(unsigned* piSegments) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MaxSegmentsCnt(unsigned* piSegments) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModeTimebaseRange(Oscb322lib_tlb::enmOscMode eMode,
                                                           unsigned long* nMinTimeBase,
                                                           unsigned long* nMaxTimeBase) = 0;
    virtual HRESULT STDMETHODCALLTYPE ModeTimebaseToFreq(Oscb322lib_tlb::enmOscMode eMode,
                                                         unsigned long nTimeBase,
                                                         double* fFreq) = 0;
    virtual HRESULT STDMETHODCALLTYPE ModeFreqToTimebase(Oscb322lib_tlb::enmOscMode eMode,
                                                         double fFreq,
                                                         unsigned long* nTimeBase) = 0;
};
typedef CComPtr<IOsc2> IOsc2Ptr;

interface IOscCalibration : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_Title(BSTR* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE Start(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Abort(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Status(short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Calibrated(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MaxProgress(unsigned long* pVal) = 0;
};
typedef CComPtr<IOscCalibration> IOscCalibrationPtr;

interface _IOscEvents : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnDataReady(unsigned long nChannelsMask) = 0;
};
typedef CComPtr<_IOscEvents> _IOscEventsPtr;

interface IDataReadyF : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnDataReadyF(VARIANT DataArray,
                                                   __int64 nFirstPointIndex) = 0;
};
typedef CComPtr<IDataReadyF> IDataReadyFPtr;

interface IStateRunEvent : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnRun(void) = 0;
};
typedef CComPtr<IStateRunEvent> IStateRunEventPtr;

interface IStateStopEvent : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnStop(void) = 0;
};
typedef CComPtr<IStateStopEvent> IStateStopEventPtr;

interface IStateErrorStopEvent : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnErrorStop(void) = 0;
};
typedef CComPtr<IStateErrorStopEvent> IStateErrorStopEventPtr;

interface IStateLooseConnectionEvent : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnLooseConnection(void) = 0;
};
typedef CComPtr<IStateLooseConnectionEvent> IStateLooseConnectionEventPtr;

interface IStateRestoreConnectionEvent : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnRestoreConnection(void) = 0;
};
typedef CComPtr<IStateRestoreConnectionEvent> IStateRestoreConnectionEventPtr;

interface IOscCalibrationRIS : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_MaxValue(short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MinValue(short* pVal) = 0;
};
typedef CComPtr<IOscCalibrationRIS> IOscCalibrationRISPtr;

interface IPropertyPageSite : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE OnStatusChange(unsigned long dwFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLocaleID(unsigned long* pLocaleID) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPageContainer(LPUNKNOWN* ppUnk) = 0;
    virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(Oscb322lib_tlb::tagMSG* pMsg) = 0;
};
typedef CComPtr<IPropertyPageSite> IPropertyPageSitePtr;

interface IPropertyPage : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE SetPageSite(Oscb322lib_tlb::IPropertyPageSite* pPageSite) = 0;
    virtual HRESULT STDMETHODCALLTYPE Activate(Oscb322lib_tlb::wireHWND hWndParent,
                                               Oscb322lib_tlb::tagRECT* pRect,
                                               long bModal) = 0;
    virtual HRESULT STDMETHODCALLTYPE Deactivate(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPageInfo(tagPROPPAGEINFO* pPageInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetObjects(unsigned long cObjects,
                                                 LPUNKNOWN* ppUnk) = 0;
    virtual HRESULT STDMETHODCALLTYPE Show(unsigned nCmdShow) = 0;
    virtual HRESULT STDMETHODCALLTYPE Move(Oscb322lib_tlb::tagRECT* pRect) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsPageDirty(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Apply(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE Help(LPWSTR pszHelpDir) = 0;
    virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(Oscb322lib_tlb::tagMSG* pMsg) = 0;
};
typedef CComPtr<IPropertyPage> IPropertyPagePtr;

interface IOscChannel : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_Enabled(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DataSizeBounds(long bMax,
                                                         unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISEnabled(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RisDataSizeBounds(long bMax,
                                                            unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TimeBaseBounds(long bMax,
                                                         unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TimeBaseToFreq(unsigned long nTimeBase,
                                                         double* pfFreq) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISTimeBaseBounds(long bMax,
                                                            unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISTimeBaseToFreq(unsigned long nTimeBase,
                                                            double* pfFreq) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_GainsCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_GainToVolts(unsigned long nGain,
                                                      double* pfVolts) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InputTypesCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InputTypeToBSTR(unsigned long nType,
                                                          BSTR* pBSTR) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PretriggerBounds(long bRIS, long bMax,
                                                           long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ZeroLevelsCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ZeroLevelToVolts(unsigned long nZeroLevel,
                                                           double* pfVolts) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CatchGlitchesEnabled(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerBounds(long bMax,
                                                        unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerLevelToVolts(long bExternal,
                                                              unsigned long nVal,
                                                              double* pfVolts) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Active(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Active(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DataSize(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_DataSize(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISDataSize(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RISDataSize(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RIS(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RIS(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TimeBase(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TimeBase(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISTimeBase(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RISTimeBase(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Gain(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Gain(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_InputType(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_InputType(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PreTrigger(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_PreTrigger(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ZeroLevel(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_ZeroLevel(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CatchGlitches(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_CatchGlitches(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerSlopeNegative(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerSlopeNegative(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerCouplingDC(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerCouplingDC(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerExternal(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerExternal(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerLevel(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerLevel(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerNormal(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerNormal(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_BitsCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ID(BSTR* pID) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_IconRes(unsigned iProcessID,
                                                  unsigned* hModule,
                                                  unsigned* dwRes) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSignalData(unsigned long nCount,
                                                    double* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDigitalData(unsigned long nCount,
                                                     unsigned long* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_PostHistoryCycles(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_PostHistoryCycles(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISFixedRAM(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RISFixedRAM(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISHistogrammCycles(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RISHistogrammCycles(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_RISHistogramm(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_RISHistogramm(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE VoltsToCode(double Volts, double* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE CodeToVolts(double Code, double* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_AverageType(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_AverageType(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_AverageNum(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_AverageNum(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Averages(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Averages(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerInputTypesCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerInputTypeToBSTR(unsigned long nType,
                                                                 BSTR* pBSTR) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerInputType(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerInputType(unsigned long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CyclicRun(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_CyclicRun(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VoltsToZeroLevel(double fVolts,
                                                           unsigned long* pZeroLev) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ProbeCoeff(double* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_ProbeCoeff(double pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerProbeCoeff(double* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerProbeCoeff(double pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TrgFilter(unsigned short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TrgFilter(unsigned short pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TrgFilterCount(unsigned short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE SigSearch(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDigitalDataEx(unsigned long nCount,
                                                       VARIANT* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_SamplingFreq(double pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SamplingFreq(double* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSignalDataEx(unsigned long nCount,
                                                      VARIANT* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SmoothSize(unsigned long* pnSmoothPoints) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_SmoothSize(unsigned long pnSmoothPoints) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_SmoothEnabled(long pbSmoothEn) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SmoothEnabled(long* pbSmoothEn) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_SmoothPassCnt(unsigned long pnPassCnt) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_SmoothPassCnt(unsigned long* pnPassCnt) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerChannel(unsigned long* pnChInd) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerChannel(unsigned long pnChInd) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TriggerACDC(long* pbOpenInput) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_TriggerACDC(long pbOpenInput) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSignalDataF4(unsigned long nCount, float* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSignalDataF4Ex(unsigned long nCount,
                                                        VARIANT* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VoltsToTriggerLevel(long bExternal,
                                                              double fVal,
                                                              unsigned long* pnCode) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_TrgFilterToBSTR(unsigned short nIndex,
                                                          BSTR* pBSTR) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Ohm50Enable(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Ohm50Enable(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_BWEnable(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_BWEnable(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VgaCoef(unsigned short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_VgaCoef(unsigned short pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_AdcCoef(unsigned short* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_AdcCoef(unsigned short pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VgaConfig(Oscb322lib_tlb::IVgaConfig** pVal) = 0;
};

interface IVgaConfig : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE _Update(void) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetActiveChannel(unsigned long channelNumber) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VgaValue(unsigned short* vga) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_VgaValue(unsigned short vga) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_AdcValue(unsigned short* dac) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_AdcValue(unsigned short dac) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MinMaxVga(unsigned short* min,
                                                    unsigned short* max) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_MinMaxAdc(unsigned short* min,
                                                    unsigned short* max) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetParams(unsigned long nCount,
                                                unsigned short* pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE WriteFile(BSTR path) = 0;
    virtual HRESULT STDMETHODCALLTYPE ReadFile(BSTR path) = 0;
    virtual HRESULT STDMETHODCALLTYPE RestoreClbr(void) = 0;
};

interface ISpecific : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_Settings(unsigned long nBuffSize,
                                                   unsigned long* nDataSize,
                                                   unsigned char* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE put_Settings(unsigned long nSize,
                                                   unsigned char* newVal) = 0;
};
typedef CComPtr<ISpecific> ISpecificPtr;

interface IOsc : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE get_ChannelsCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Channel(unsigned long nChannel,
                                                  Oscb322lib_tlb::IOscChannel** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DeviceID(BSTR* pbstrID) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_HardwareID(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Run(long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_Run(long pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_CalibrationsCount(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Calibration(unsigned long nIndex,
                                                      Oscb322lib_tlb::IOscCalibration** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Location(BSTR* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Port(unsigned long* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Version(BSTR* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Interface(BSTR* pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_Specific(Oscb322lib_tlb::ISpecific** pVal) = 0;
    virtual HRESULT STDMETHODCALLTYPE SigSearch(unsigned short nChanMask) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_ReadyEventHandle(unsigned long* pEventHandle) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_ReadyEventHandle(unsigned long pEventHandle) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_VgaConfig(Oscb322lib_tlb::IVgaConfig** pVal) = 0;
};

interface IDAC : public Oscb322lib_tlb::ISpecific {
public:
    virtual HRESULT STDMETHODCALLTYPE set_DAC_Continuous(long bIsContinuous) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_DAC_Run(long bRun) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_DAC_TriggerNormal(long bTrigger) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_DAC_Freq(unsigned long nFreq) = 0;
    virtual HRESULT STDMETHODCALLTYPE set_DAC_Data(unsigned long nMemSize,
                                                   unsigned short* Param2) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_Continuous(long* bIsContinuous) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_MemSize(unsigned long* nMemSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_Run(long* bRun) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_TriggerNormal(long* bTrigger) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_Freq(unsigned long* nFreq) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_MaxCode(unsigned short* nMaxCode) = 0;
    virtual HRESULT STDMETHODCALLTYPE VoltsToCode(double dVolts,
                                                  unsigned short* nCode) = 0;
    virtual HRESULT STDMETHODCALLTYPE CodeToVolts(unsigned short nCode,
                                                  double* dVolts) = 0;
    virtual HRESULT STDMETHODCALLTYPE DAC_SetConstLevel(double dLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE DAC_SetSin(double dOffset, double dAmplitude,
                                                 double dPeriod, double dFaza) = 0;
    virtual HRESULT STDMETHODCALLTYPE DAC_SetTriang(double dOffset, double dPeakToPeak,
                                                    double dPeriod, double dDutyCycle) = 0;
    virtual HRESULT STDMETHODCALLTYPE DAC_SetMeandr(double dOffset, double dPeakToPeak,
                                                    double dPeriod, double dDutyCycle) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_LastConfig(long* LastConfig,
                                                         double* dOffset,
                                                         double* dAmplitude,
                                                         double* dPeriod,
                                                         double* dExtPar) = 0;
    virtual HRESULT STDMETHODCALLTYPE get_DAC_Data(unsigned long* nMaxMemSize,
                                                   unsigned short* nData) = 0;
    virtual HRESULT STDMETHODCALLTYPE DAC_SetUserDefined(unsigned long nLeng,
                                                         signed char* sName) = 0;
};
typedef CComPtr<IDAC> IDACPtr;

typedef IOscChannel OscChannelB322;
typedef IOscChannelPtr OscChannelB322Ptr;
typedef IOsc OscDeviceB322;
typedef IOscPtr OscDeviceB322Ptr;
typedef IOsc OscDeviceB322Im;
typedef IOscPtr OscDeviceB322ImPtr;
typedef IOscCalibration ZeroLevClbr;
typedef IOscCalibrationPtr ZeroLevClbrPtr;
typedef IUnknown ZeroLevPPage;
typedef IUnknownPtr ZeroLevPPagePtr;
typedef IOscCalibration RisClbr;
typedef IOscCalibrationPtr RisClbrPtr;
typedef IUnknown RisClbrPPage;
typedef IUnknownPtr RisClbrPPagePtr;
typedef IOscCalibration SyncClbr;
typedef IOscCalibrationPtr SyncClbrPtr;
typedef IOscCalibration TrigCalibration;
typedef IOscCalibrationPtr TrigCalibrationPtr;
typedef IPropertyPage TrigPPage;
typedef IPropertyPagePtr TrigPPagePtr;
typedef IDAC dac;
typedef IDACPtr dacPtr;
typedef IUnknown DAC_PPage;
typedef IUnknownPtr DAC_PPagePtr;

#define LIBID_OF_OscChannelB322 (&LIBID_OscB322Lib)
#define LIBID_OF_OscDeviceB322 (&LIBID_OscB322Lib)
#define LIBID_OF_OscDeviceB322Im (&LIBID_OscB322Lib)
#define LIBID_OF_ZeroLevClbr (&LIBID_OscB322Lib)
#define LIBID_OF_ZeroLevPPage (&LIBID_OscB322Lib)
#define LIBID_OF_RisClbr (&LIBID_OscB322Lib)
#define LIBID_OF_RisClbrPPage (&LIBID_OscB322Lib)
#define LIBID_OF_SyncClbr (&LIBID_OscB322Lib)
#define LIBID_OF_TrigCalibration (&LIBID_OscB322Lib)
#define LIBID_OF_TrigPPage (&LIBID_OscB322Lib)
#define LIBID_OF_dac (&LIBID_OscB322Lib)
#define LIBID_OF_DAC_PPage (&LIBID_OscB322Lib)
};     // namespace Oscb322lib_tlb

#endif // OscB322Lib_TLBH
