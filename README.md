# simple-DLLInjector

A lightweight and educational DLL injector written in C++ (x64), designed for Windows-based systems. This project demonstrates a basic but fully functional approach to DLL injection using `OpenProcess`, `VirtualAllocEx`, `WriteProcessMemory`, and `CreateRemoteThread`.

🛠️ **What it does**:
- Prompts the user for a target process PID
- Allocates memory in the remote process
- Writes a DLL path to the allocated memory
- Loads the DLL using `LoadLibraryA` via a remote thread

📌 **Why it matters**:
This project reflects early experience in low-level Windows internals and reverse engineering. Written at age 15 as part of a learning path towards security and system-level programming.

## ⚠️ Disclaimer

This project is strictly for educational purposes. Use responsibly. I do not condone cheating in games or malicious use of code.

## 🔧 How to use

1. Compile in `Release x64`.
2. Provide a valid DLL path in the source code.
3. Run the injector and input the target PID.

---
