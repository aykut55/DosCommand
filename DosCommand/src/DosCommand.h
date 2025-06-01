
#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <windows.h>

class CDosCommand 
{
public:
    using OutputCallback = std::function<void(const std::string&)>;

    virtual ~CDosCommand();
             CDosCommand();

    bool Start(const std::string& command, OutputCallback callback);
    void WriteInput(const std::string& input);
    void Stop(bool force = false);
    bool IsRunning() const;

    bool WaitForExit(int timeoutMs = INFINITE);
    int GetExitCode() const;

    // Manüel cikti okuma fonksiyonu
    bool ReadOutputLine(std::string& line);

private:
    void ReadOutputLoop();
    int getExitCode() const;

    HANDLE hChildStd_IN_Wr = nullptr;
    HANDLE hChildStd_OUT_Rd = nullptr;
    PROCESS_INFORMATION piProcInfo = { 0 };
    std::thread outputThread;
    bool running = false;
    OutputCallback onOutput;
    bool isThreadExitRequested = false;
    int dosCommandExitCode = 0;

    std::mutex outputMutex;
    std::queue<std::string> outputQueue;
};
