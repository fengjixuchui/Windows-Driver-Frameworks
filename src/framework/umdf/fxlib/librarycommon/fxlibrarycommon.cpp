//
//    Copyright (C) Microsoft.  All rights reserved.
//
#include <ntverp.h>
#include <strsafe.h>
#include <driverspecs.h>

extern "C" {
#include "mx.h"
}
#include "fxmin.hpp"
#include <fxldrUm.h>

#include <wdfcxbase.h>
#include "wdf20.h"
#include "wdf215.h"
#include "wdf217.h"
#include "wdf219.h"
#include "wdf221.h"
#include "wdf223.h"
#include "wdf225.h"

//
// This will cause inclusion of VfWdfFunctions table implementation from header
//
#define  VF_FX_DYNAMICS_GENERATE_TABLE   1





#include "FxDynamics.h"
#include "vffxdynamics.h"
#include "FxLibraryCommon.h"

#include "FxTelemetry.hpp"

extern "C" {
//
// Global triage Info for dbgeng and 0x9F work
//
static WDFOBJECT_TRIAGE_INFO       _WdfObjectTriageInfo = {0};
static WDFCONTEXT_TRIAGE_INFO      _WdfContextTriageInfo = {0};
static WDFCONTEXTTYPE_TRIAGE_INFO  _WdfContextTypeTriageInfo = {0};
static WDFQUEUE_TRIAGE_INFO        _WdfQueueTriageInfo = {0};
static WDFIRPQUEUE_TRIAGE_INFO     _WdfIrpQueueTriageInfo = {0};
static WDFREQUEST_TRIAGE_INFO      _WdfRequestTriageInfo = {0};
static WDFDEVICE_TRIAGE_INFO       _WdfDeviceTriageInfo = {0};
static WDFIRP_TRIAGE_INFO          _WdfIrpTriageInfo = {0};
static WDFFWDPROGRESS_TRIAGE_INFO  _WdfFwdProgressTriageInfo = {0};

WDF_TRIAGE_INFO g_WdfTriageInfo = {
    //
    // UMDF Version.
    //
    __WUDF_MAJOR_VERSION,
    __WUDF_MINOR_VERSION,

    //
    // Table Version.
    //
    WDF_01_TRIAGE_INFO_MAJOR_VERSION,
    WDF_01_TRIAGE_INFO_MINOR_VERSION,

    //
    // Reserved ptr (set to NULL).
    //
    NULL,

    //
    // WDF objects triage info.
    //
    &_WdfObjectTriageInfo,
    &_WdfContextTriageInfo,
    &_WdfContextTypeTriageInfo,
    &_WdfQueueTriageInfo,
    &_WdfFwdProgressTriageInfo,
    &_WdfIrpQueueTriageInfo,
    &_WdfRequestTriageInfo,
    &_WdfDeviceTriageInfo,
    &_WdfIrpTriageInfo,
};
} // extern "C"

VOID
GetTriageInfo(
    VOID
    )
{
    // Object
    _WdfObjectTriageInfo.RawObjectSize = sizeof(FxObject);
    _WdfObjectTriageInfo.ObjectType = FIELD_OFFSET(FxObject, m_Type);
    _WdfObjectTriageInfo.TotalObjectSize = FIELD_OFFSET(FxObject, m_ObjectSize);
    _WdfObjectTriageInfo.ChildListHead = FIELD_OFFSET(FxObject, m_ChildListHead);
    _WdfObjectTriageInfo.ChildEntry = FIELD_OFFSET(FxObject, m_ChildEntry);
    _WdfObjectTriageInfo.Globals = FIELD_OFFSET(FxObject, m_Globals);
    _WdfObjectTriageInfo.ParentObject = FIELD_OFFSET(FxObject, m_ParentObject);

    // Context Triage Info
    _WdfContextTriageInfo.HeaderSize = sizeof(FxContextHeader);
    _WdfContextTriageInfo.NextHeader = FIELD_OFFSET(FxContextHeader, NextHeader);
    _WdfContextTriageInfo.Object = FIELD_OFFSET(FxContextHeader, Object);
    _WdfContextTriageInfo.TypeInfoPtr = FIELD_OFFSET(FxContextHeader, ContextTypeInfo);
    _WdfContextTriageInfo.Context = FIELD_OFFSET(FxContextHeader, Context);

    // Context type Triage info
    _WdfContextTypeTriageInfo.TypeInfoSize = sizeof(WDF_OBJECT_CONTEXT_TYPE_INFO);
    _WdfContextTypeTriageInfo.ContextSize = FIELD_OFFSET(WDF_OBJECT_CONTEXT_TYPE_INFO, ContextSize);
    _WdfContextTypeTriageInfo.ContextName = FIELD_OFFSET(WDF_OBJECT_CONTEXT_TYPE_INFO, ContextName);

    // WdfRequest Queue
    _WdfQueueTriageInfo.QueueSize = sizeof(FxIoQueue);
    _WdfQueueTriageInfo.IrpQueue1 = FIELD_OFFSET(FxIoQueue, m_Queue);
    _WdfQueueTriageInfo.IrpQueue2 = FIELD_OFFSET(FxIoQueue, m_DriverCancelable);
    _WdfQueueTriageInfo.RequestList1 = FIELD_OFFSET(FxIoQueue, m_Cancelled);
    _WdfQueueTriageInfo.RequestList2 = FIELD_OFFSET(FxIoQueue, m_CanceledOnQueueList);
    _WdfQueueTriageInfo.FwdProgressContext = FIELD_OFFSET(FxIoQueue, m_FwdProgContext);
    _WdfQueueTriageInfo.PkgIo = FIELD_OFFSET(FxIoQueue, m_PkgIo);

    // Forward Progress
    _WdfFwdProgressTriageInfo.ReservedRequestList =
        FIELD_OFFSET(FXIO_FORWARD_PROGRESS_CONTEXT, m_ReservedRequestList);
    _WdfFwdProgressTriageInfo.ReservedRequestInUseList =
        FIELD_OFFSET(FXIO_FORWARD_PROGRESS_CONTEXT, m_ReservedRequestInUseList);
    _WdfFwdProgressTriageInfo.PendedIrpList =
        FIELD_OFFSET(FXIO_FORWARD_PROGRESS_CONTEXT, m_PendedIrpList);

    // Irp Queue
    _WdfIrpQueueTriageInfo.IrpQueueSize = sizeof(FxIrpQueue);
    _WdfIrpQueueTriageInfo.IrpListHeader = FIELD_OFFSET(FxIrpQueue, m_Queue);









    _WdfIrpQueueTriageInfo.IrpListEntry = 0;
    _WdfIrpQueueTriageInfo.IrpContext = 0;

    // WdfRequest
    _WdfRequestTriageInfo.RequestSize = sizeof(FxRequest);
    _WdfRequestTriageInfo.CsqContext = FIELD_OFFSET(FxRequest, m_CsqContext);
    _WdfRequestTriageInfo.FxIrp = FIELD_OFFSET(FxRequest, m_Irp);
    _WdfRequestTriageInfo.ListEntryQueueOwned =
        FIELD_OFFSET(FxRequest, m_OwnerListEntry);
    _WdfRequestTriageInfo.ListEntryQueueOwned2 =
        FIELD_OFFSET(FxRequest, m_OwnerListEntry2);
    _WdfRequestTriageInfo.RequestListEntry =
        FIELD_OFFSET(FxRequest, m_ListEntry);
    _WdfRequestTriageInfo.FwdProgressList =
        FIELD_OFFSET(FxRequest, m_ForwardProgressList);

    // WdfDevice
    _WdfDeviceTriageInfo.DeviceInitSize = sizeof(WDFDEVICE_INIT);
    _WdfDeviceTriageInfo.DeviceDriver = FIELD_OFFSET(FxDevice, m_Driver);

    // FxIrp
    _WdfIrpTriageInfo.FxIrpSize = sizeof(FxIrp);
    _WdfIrpTriageInfo.IrpPtr = FIELD_OFFSET(FxIrp, m_Irp);
}


BOOLEAN
IsClientInfoValid(
    _In_ PCLIENT_INFO ClientInfo
    )
{
    if (ClientInfo == NULL ||
        ClientInfo->Size != sizeof(CLIENT_INFO) ||
        ClientInfo->RegistryPath == NULL ||
        ClientInfo->RegistryPath->Length == 0 ||
        ClientInfo->RegistryPath->Buffer == NULL) {
        return FALSE;
    }
    return TRUE;
}

VOID
ReportDdiFunctionCountMismatch(
    _In_ PCUNICODE_STRING ServiceName,
    _In_ ULONG ActualFunctionCount,
    _In_ ULONG ExpectedFunctionCount,
    _In_ BOOLEAN IssueBreak
    )
{
    WCHAR serviceName[MAX_PATH] = { 0 };
    char  serviceNameAnsi[MAX_PATH] = { 0 };
    int nChars;

    //
    // NOTE: Any single call to DbgPrintEx will only transmit 512 bytes of
    // information.
    //
    DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL,
            "\n\n************************* \n"
            "* DDI function table mismatch detected in UMDF driver. The \n"
            "* driver will not load until it is re-compiled using a \n"
            "* newer version of the Windows Driver Kit (WDK). \n"
            );

    DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL,
            "* Service name                 : %wZ\n"
            "* Actual function table count  : %d \n"
            "* Expected function table count: %d \n"
            "*************************** \n\n",
            ServiceName,
            ActualFunctionCount,
            ExpectedFunctionCount
            );

    //
    // Write an ETW event to the system event log.
    //
    RtlCopyMemory(serviceName,
                ServiceName->Buffer,
                MIN(sizeof(serviceName), ServiceName->Length));
    EventWriteEVENT_UMDF_DRIVER_DDI_TABLE_MISMATCH(serviceName,
                                                ActualFunctionCount,
                                                ExpectedFunctionCount);

    //
    // Report a telemetry event that can be used to proactively fix drivers
    //
    TraceLoggingWrite(g_TelemetryProvider,
                    "UmdfClientFunctionCountMismatch",
                    WDF_TELEMETRY_EVT_KEYWORDS,
                    TraceLoggingUnicodeString(ServiceName, "ServiceName"),
                    TraceLoggingUInt32(ActualFunctionCount, "FunctionCount"),
                    TraceLoggingUInt32(ExpectedFunctionCount, "ExpectedCount"));

    if (IssueBreak == TRUE) {
        //
        // Convert to an ANSI string and generate a verifier failure so a Watson
        // report is submitted
        //
        nChars = WideCharToMultiByte(CP_ACP,
                                    WC_NO_BEST_FIT_CHARS, //flags
                                    serviceName,
                                    -1,
                                    serviceNameAnsi,
                                    sizeof(serviceNameAnsi),
                                    NULL, NULL);

        FX_VERIFY_WITH_NAME(DRIVER(DDIFunctionTableMismatched, 0),
            TRAPMSG("Version mismatch detected in function table count. Recompile"
            " driver with correct headers"), nChars ? serviceNameAnsi : WDF_UNKNOWN_SERVICE_NAME);
    }
}

NTSTATUS
FxLibraryCommonCommission(
    VOID
    )
{
    NTSTATUS   status;

    __Print((LITERAL(WDF_LIBRARY_COMMISSION) "\n"));

    //
    // Commission this version's DLL globals.
    //
    status = FxLibraryGlobalsCommission();
    if (!NT_SUCCESS(status)) {
        __Print(("FxLibraryGlobalsCommission failed %X\n", status));
        return status;
    }

    //
    // register for ETW tracing.
    //
    RegisterTelemetryProvider();

    //
    // Initialize internal WPP tracing.
    //
    status = FxTraceInitialize();
    if (NT_SUCCESS(status)) {
        FxLibraryGlobals.InternalTracingInitialized = TRUE;
    }
    else {
        __Print(("Failed to initialize tracing for WDF\n"));

        //
        // Failure to initialize is not critical enough to fail driver load.
        //
        status = STATUS_SUCCESS;
    }

    return status;
}

NTSTATUS
FxLibraryCommonDecommission(
    VOID
    )
{
    __Print((LITERAL(WDF_LIBRARY_DECOMMISSION) ": enter\n"));

    //
    // Uninitialize WPP tracing.
    //
    if (FxLibraryGlobals.InternalTracingInitialized) {
        TraceUninitialize();
        FxLibraryGlobals.InternalTracingInitialized = FALSE;
    }

    //
    // Unregister telemetry provider.
    //
    UnregisterTelemetryProvider();

    EventUnregisterMicrosoft_Windows_DriverFrameworks_UserMode_Performance();

    //
    // Decommission this version's DLL globals.
    //
    FxLibraryGlobalsDecommission();

    //
    // Note: This is the absolute last action from WDF library (dynamic or static).
    //       The image is likely to be deleted after returning.
    //
    __Print((LITERAL(WDF_LIBRARY_DECOMMISSION) ": exit\n"));

    return STATUS_SUCCESS;
}


NTSTATUS
FxLibraryCommonRegisterClient(
    PWDF_BIND_INFO        Info,
    PWDF_DRIVER_GLOBALS * WdfDriverGlobals,
    PCLIENT_INFO          ClientInfo
    )
{
    NTSTATUS           status;
    UNICODE_STRING     serviceName = { 0 };

    status = STATUS_INVALID_PARAMETER;

    __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT) ": enter\n"));

    ASSERT(Info != NULL && Info->FuncCount != 0);
    ASSERT(WdfDriverGlobals != 0);

    if (Info == NULL || WdfDriverGlobals == NULL || Info->FuncTable == NULL) {
        __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                 ": NULL parameter -- %s\n",
                 (Info == NULL)             ? "PWDF_BIND_INFO" :
                 (WdfDriverGlobals == NULL) ? "PWDF_DRIVER_GLOBALS *" :
                 (Info->FuncTable == NULL)  ? "PWDF_BIND_INFO->FuncTable" :
                                              "unknown" ));
        goto Done;
    }

    *WdfDriverGlobals = NULL;

    ASSERT(Info->FuncCount <= WdfVersion.FuncCount);

    //
    // WdfVersion.Count is initialized in FxDynamics.h and is never changed.
    // Prefast is unable to make that determination.
    //
    __assume(WdfVersion.FuncCount == sizeof(WDFFUNCTIONS)/sizeof(PVOID));

    if (Info->FuncCount > WdfVersion.FuncCount) {
        __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                 ": version mismatch detected in function table count: client"
                 "has 0x%x,  library has 0x%x\n",
                 Info->FuncCount, WdfVersion.FuncCount));
        goto Done;
    }

    if (Info->FuncCount <= WdfFunctionTableNumEntries_V2_25) {

        switch (Info->FuncCount) {

        case WdfFunctionTableNumEntries_V2_25: // 268 - win10 1803 RS4
        case WdfFunctionTableNumEntries_V2_23: // 265 - win10 1709 RS3
        case WdfFunctionTableNumEntries_V2_21: // 261 - win10 1703 RS2
     // case WdfFunctionTableNumEntries_V2_19: // 261 - win10 1607 RS1
        case WdfFunctionTableNumEntries_V2_17: // 260 - win10 1511 TH2
        case WdfFunctionTableNumEntries_V2_15: // 257 - win10 1507 TH1
        case WdfFunctionTableNumEntries_V2_0:  // 248 - win8.1
            break;

        default:
            __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                     ": Function table count 0x%x doesn't match any previously "
                     "released framework version table size\n",
                     Info->FuncCount));
            goto Done;
        }
    }
    else {






        //
        // Client version is same as framework version. Make
        // sure table count is exact.
        //
        BOOLEAN issueBreak = WDF_PRODUCTION_RELEASE;

        if (Info->FuncCount != WdfFunctionTableNumEntries) {
            RtlInitUnicodeString(&serviceName, WIDEN(WDF_UNKNOWN_SERVICE_NAME));

            if (IsClientInfoValid(ClientInfo)) {
                GetNameFromPath(ClientInfo->RegistryPath, &serviceName);
            }

            //
            // Report a DbgPrint message, telemetry event and an ETW event that
            // will serve as diagnostic aid.
            //
            ReportDdiFunctionCountMismatch((PCUNICODE_STRING)&serviceName,
                                           Info->FuncCount,
                                           WdfFunctionTableNumEntries,
                                           issueBreak);
#if WDF_PRODUCTION_RELEASE
            goto Done;
#endif
        }
    }

    //
    // Allocate an new FxDriverGlobals area for this driver.
    //
    *WdfDriverGlobals = FxAllocateDriverGlobals();

    if (*WdfDriverGlobals) {
        BOOLEAN isFunctinTableHookingOn  = FALSE;
        BOOLEAN isPerformanceAnalysisOn  = FALSE;
        PFX_DRIVER_GLOBALS fxDriverGlobals = NULL;
        //
        // Check the registry to see if Enhanced verifier is on for this driver.
        // if registry read fails, options value remains unchanged.
        // store enhanced verifier options in driver globals
        //
        fxDriverGlobals = GetFxDriverGlobals(*WdfDriverGlobals);
        GetEnhancedVerifierOptions(ClientInfo, &fxDriverGlobals->FxEnhancedVerifierOptions);
        isFunctinTableHookingOn = IsFxVerifierFunctionTableHooking(fxDriverGlobals);
        isPerformanceAnalysisOn = IsFxPerformanceAnalysis(fxDriverGlobals);

        //
        // Set-up the function table. Enhanced verifier and Performance analysis is off by default.
        //
        if (isFunctinTableHookingOn == FALSE && isPerformanceAnalysisOn == FALSE) {
            //
            // Set-up the function table
            //
            // Starting in 2.15 we reference a copy of the DDI table in WDF01000,
            // prior to that we copy the entire table to local memory.
            //
            if (Info->FuncCount <= WdfFunctionTableNumEntries_V2_0) {
                RtlCopyMemory( Info->FuncTable,
                               &WdfVersion.Functions,
                               Info->FuncCount * sizeof(PVOID) );
            }
            else {
                //
                // FuncTable arrives with a ptr to &WdfFunctions, so we update
                // what WdfFunctions points to
                //
                *((WDFFUNC**) Info->FuncTable) = (WDFFUNC*) &WdfVersion.Functions;
            }
        }
        else {
            __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                     ": Enhanced Verification is ON \n"));

            if (Microsoft_Windows_DriverFrameworks_UserMode_PerformanceHandle == NULL)
            {
                EventRegisterMicrosoft_Windows_DriverFrameworks_UserMode_Performance();
            }

            //
            // Enhanced verification is on. Return verifier function table
            //
            // Starting in 1.15 we reference a copy of the DDI table in WDF01000,
            // prior to that we copy the entire table to local memory.
            //
            if (Info->FuncCount <= WdfFunctionTableNumEntries_V2_0) {
                RtlCopyMemory( Info->FuncTable,
                               &VfWdfVersion.Functions,
                               Info->FuncCount * sizeof(PVOID) );
            }
            else {
                //
                // FuncTable arrives with a ptr to &WdfFunctions, so we update
                // what WdfFunctions points to.
                //
                *((WDFFUNC**) Info->FuncTable) = (WDFFUNC*) &VfWdfVersion.Functions;
            }
        }

        status = STATUS_SUCCESS;

        __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                 ": WdfFunctions %p\n", Info->FuncTable));
    }

Done:
    __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
             ": exit: status %X\n", status));

    if (!NT_SUCCESS(status)) {
        FX_VERIFY(DRIVER(DDIFunctionTableMismatched, TODO),
            TRAPMSG("Version mismatch detected in function table count. Recompile"
            " driver with correct headers"));
    }

    return status;
}

NTSTATUS
FxLibraryCommonUnregisterClient(
    PWDF_BIND_INFO        Info,
    PWDF_DRIVER_GLOBALS   WdfDriverGlobals
    )
{
    NTSTATUS status;

    __Print((LITERAL(WDF_LIBRARY_UNREGISTER_CLIENT) ": enter\n"));

    ASSERT(Info != NULL);
    ASSERT(WdfDriverGlobals != NULL);

    if (Info != NULL && WdfDriverGlobals != NULL) {
        PFX_DRIVER_GLOBALS pFxDriverGlobals;

        status = STATUS_SUCCESS;

        pFxDriverGlobals = GetFxDriverGlobals(WdfDriverGlobals);

        //
        // Destroy this FxDriver instance, if its still indicated.
        //
        if (pFxDriverGlobals->Driver != NULL) {
            //
            // Association support, we are a root with no parent
            //
            pFxDriverGlobals->Driver->DeleteObject();

            FxDestroy(pFxDriverGlobals);
        }

        //
        // Stop IFR logging
        //
        FxIFRStop(pFxDriverGlobals);

        //
        // This will free the client's FxDriverGlobals area
        //
        FxFreeDriverGlobals(WdfDriverGlobals);
    }
    else {
        status = STATUS_UNSUCCESSFUL;
    }

    __Print((LITERAL(WDF_LIBRARY_UNREGISTER_CLIENT)
             ": exit: status %X\n", status));

    return status;
}

VOID
GetEnhancedVerifierOptions(
    PCLIENT_INFO ClientInfo,
    PULONG Options
    )
{
    NTSTATUS status;
    ULONG value = 0;
    FxAutoRegKey hServiceKey, hWdfSubkey;
    DECLARE_CONST_UNICODE_STRING(parametersPath, L"Parameters\\Wdf");
    DECLARE_CONST_UNICODE_STRING(valueName, WDF_ENHANCED_VERIFIER_OPTIONS_VALUE_NAME);

    *Options = 0;
    if (! IsClientInfoValid(ClientInfo) ||
        Options == NULL) {

        __Print((LITERAL(WDF_LIBRARY_REGISTER_CLIENT)
                 ": Invalid ClientInfo received from wudfldr \n"));
        return;
    }

    status = FxRegKey::_OpenKey(NULL,
                                ClientInfo->RegistryPath,
                                &hServiceKey.m_Key,
                                KEY_READ);
    if (!NT_SUCCESS(status)) {
        return;
    }

    status = FxRegKey::_OpenKey(hServiceKey.m_Key,
                                &parametersPath,
                                &hWdfSubkey.m_Key,
                                KEY_READ);
    if (NT_SUCCESS(status)) {
        //
        // Try to read the EnhancedVerifierOptions value
        // from the service key's Parameters\Wdf subkey.
        //
        status = FxRegKey::_QueryULong(
            hWdfSubkey.m_Key, &valueName, &value);
    }

    if (!NT_SUCCESS(status)) {
        //
        // Older versions of WdfPerfEnhancedVerifier.cmd set the
        // EnhancedVerifierOptions value to the UMDF client's root
        // service key instead of the Parameters\Wdf subkey, so in
        // case of failure also check there.
        //
        status = FxRegKey::_QueryULong(
            hServiceKey.m_Key, &valueName, &value);
    }

    //
    // Examine key values and set Options only on success.
    //
    if (NT_SUCCESS(status)) {
        if (value) {
            *Options = value;
        }
    }
}
