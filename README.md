### SOURCE CODE ###
Source codes inside the src/ Directory.

### Executable/DLL Files ###
Executables/DLLs Files inside the bin/ Directory.

### How to run? ###
Run: WaitingHook.exe and then run Hooker.exe passing the corrects parameters, i.e: Hooker.exe <process.exe> <C:\Path\To\DLL.dll>

### How does it works? ###
It seeks for the desired process's thread, then gets its TID (Thread ID), open the thread's handle, open the process itself, inject the desired dll within and waits until the main process's thread enters in an alertable state, when it happens, it will instead execute our code, once we inserted the code to be executed via QueueUserAPC.
