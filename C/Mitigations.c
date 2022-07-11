#include "Precomp.h"

#ifdef _WIN32

#include <Windows.h>
#include <VersionHelpers.h>

#include "Mitigations.h"

static BOOL VerifyWindowsBuild(DWORD major, DWORD minor, DWORD build) {
    OSVERSIONINFOEXW vi = {0};
    vi.dwMajorVersion = major;
    vi.dwMinorVersion = minor;
    vi.dwBuildNumber = build;
    DWORDLONG vcond = 0;
    vcond = VerSetConditionMask(vcond, VER_MAJORVERSION, VER_GREATER_EQUAL);
    vcond = VerSetConditionMask(vcond, VER_MINORVERSION, VER_GREATER_EQUAL);
    vcond = VerSetConditionMask(vcond, VER_BUILDNUMBER, VER_GREATER_EQUAL);
    return VerifyVersionInfoW(&vi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, vcond);
}

BOOL EnableMitigations() {
    if (!HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0))
        return FALSE;

    if (IsWindows8OrGreater()) {
        PROCESS_MITIGATION_STRICT_HANDLE_CHECK_POLICY shcp = {0};
        shcp.RaiseExceptionOnInvalidHandleReference = 1;
        shcp.HandleExceptionsPermanentlyEnabled = 1;
        if (!SetProcessMitigationPolicy(ProcessStrictHandleCheckPolicy, &shcp, sizeof(shcp)))
            return FALSE;
    }

    if (IsWindows8Point1OrGreater()) {
        PROCESS_MITIGATION_DYNAMIC_CODE_POLICY dcp = {0};
        dcp.ProhibitDynamicCode = 1;
        if (!SetProcessMitigationPolicy(ProcessDynamicCodePolicy, &dcp, sizeof(dcp)))
            return FALSE;
    }

    if (IsWindows10OrGreater()) {
        PROCESS_MITIGATION_IMAGE_LOAD_POLICY Policy = {0};
        Policy.NoRemoteImages = 1;
        Policy.NoLowMandatoryLabelImages = 1;
        if (!SetProcessMitigationPolicy(ProcessImageLoadPolicy, &Policy, sizeof(PROCESS_MITIGATION_IMAGE_LOAD_POLICY)))
            return FALSE;
    }

    if (VerifyWindowsBuild(10, 0, 19041)) {
        PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY ussp = {0};
        if (GetProcessMitigationPolicy(GetCurrentProcess(), ProcessUserShadowStackPolicy, &ussp, sizeof(ussp)) &&
            ussp.EnableUserShadowStack) {
            ussp.EnableUserShadowStackStrictMode = 1;
            if (!SetProcessMitigationPolicy(ProcessUserShadowStackPolicy, &ussp, sizeof(ussp))) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL EnableChildProcessMitigation() {
    if (VerifyWindowsBuild(10, 0, 16299)) {
        PROCESS_MITIGATION_CHILD_PROCESS_POLICY cpp = {0};
        cpp.NoChildProcessCreation = 1;
        if (!SetProcessMitigationPolicy(ProcessChildProcessPolicy, &cpp, sizeof(cpp))) {
            return FALSE;
        }
    }

    return TRUE;
}

#endif
