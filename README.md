# 🔍 Endpoint Detection & Response Simulation Toolkit

This repository contains **three self-contained simulation tools** that mimic common attacker behaviors for **EDR/SIEM detection testing** and **security training labs**.
They are **not** real malware — all actions are simulated and safe for isolated environments.

⚠ **WARNING**:
Run **only** in a controlled lab or virtual machine.
Do **not** use on production systems or against unauthorized targets.

---

## 📂 Included Tasks

### 1️⃣ Initial Execution & Evasion

**File:** `task1_initial_execution_evasion.cpp`
**Binary:** `task1_initial_execution_evasion.exe`

**Simulated Techniques:**

* XOR-encoded strings (basic obfuscation)
* Junk code placeholder
* Mock AMSI bypass (patches `AmsiScanBuffer` to immediately return)

**Detection Goal:**
Test security controls for **obfuscated code execution** and AMSI tampering.

---

### 2️⃣ Privilege Escalation & Credential Access

**File:** `task2_privilege_escalation_credential_access.cpp`
**Binary:** `task2_privilege_escalation_credential_access.exe`

**Simulated Techniques:**

* **Mock SAM extraction:** `reg save HKLM\\SAM`
* **Fake LSASS dump:** Creates a dummy file with a fake PID reference
* **Simulated SYSTEM service creation:** Uses `sc create`

**Detection Goal:**
Trigger alerts for **high-privilege commands** and **credential theft behavior**.

---

### 3️⃣ File Encryption Simulation (Ransomware Behavior)

**File:** `task3_ransomware_behavior_simulation.cpp`
**Binary:** `task3_ransomware_behavior_simulation.exe`

**Simulated Techniques:**

* Encrypts files via **reverse + Base64 encoding**
* Creates ransom note in every folder
* Logs all file changes to `sim_encryption_log.txt`

**Detection Goal:**
Detect mass file modification patterns similar to ransomware.

---

## ⚙️ Compilation

Compile using **Visual Studio Developer Command Prompt** or any C++17+ compiler.

Example (MinGW):

```bash
g++ task1_initial_execution_evasion.cpp -o task1_initial_execution_evasion.exe -static
g++ task2_privilege_escalation_credential_access.cpp -o task2_privilege_escalation_credential_access.exe -static
g++ task3_ransomware_behavior_simulation.cpp -o task3_ransomware_behavior_simulation.exe -static
```

---

## 🛡 Usage & Safety

* Recommended: Run in an **isolated lab VM**
* Adjust file paths in source code to point to **test directories**
* All sensitive actions (credential dumps, encryption) are **simulated**, not real

---

## 📜 License

MIT License – You are free to use, modify, and share for **research, training, and defensive testing**.

---

## ⚠ Disclaimer

The author is **not responsible** for misuse.
These programs are intended **solely for educational and security testing purposes**.
