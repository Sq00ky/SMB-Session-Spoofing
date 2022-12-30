#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "advapi32.lib")

using namespace std;
int main() {
    while (true) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        si.wShowWindow = 0;
        si.dwFlags = 0x00000001;
        wchar_t cmdLine[] = L"'C:\\Windows\\System32\\net.exe' use \\\\10.0.0.70\\c$";
        CreateProcessWithLogonW(L"ea-patrick", L"contoso.com", NULL, LOGON_NETCREDENTIALS_ONLY, L"C:\\Windows\\System32\\net.exe", cmdLine, NULL, NULL, NULL, &si, &pi);
        Sleep(750000);
    }
}