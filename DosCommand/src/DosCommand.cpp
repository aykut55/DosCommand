
#include "DosCommand.h"
#include <iostream>

CDosCommand::~CDosCommand()
{
    if (running) {
        Stop();
    }

    if (outputThread.joinable())
    {
        if (!isThreadExitRequested)
        {
            while (!isThreadExitRequested)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        outputThread.join();
    }
}

CDosCommand::CDosCommand() 
{

}

bool CDosCommand::Start(const std::string& command, OutputCallback callback) 
{
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    HANDLE hChildStd_OUT_Wr = nullptr, hChildStd_IN_Rd = nullptr;

    if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) return false;
    if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) return false;
    if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0)) return false;
    if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) return false;

    STARTUPINFOA siStartInfo = {};
    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdError = hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = hChildStd_OUT_Wr;
    siStartInfo.hStdInput = hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    std::string cmd = "cmd.exe /C " + command;
    char* cmdline = _strdup(cmd.c_str());

    BOOL success = CreateProcessA(
        nullptr, cmdline, nullptr, nullptr, TRUE,
        0, nullptr, nullptr, &siStartInfo, &piProcInfo);

    free(cmdline);
    CloseHandle(hChildStd_OUT_Wr);
    CloseHandle(hChildStd_IN_Rd);

    if (!success) return false;

    running = true;
    onOutput = callback;
    outputThread = std::thread(&CDosCommand::ReadOutputLoop, this);
    return true;
}

void CDosCommand::WriteInput(const std::string& input) 
{
    if (hChildStd_IN_Wr && running) {
        DWORD written;
        std::string data = input + "\n";
        WriteFile(hChildStd_IN_Wr, data.c_str(), (DWORD)data.size(), &written, nullptr);
    }
}

void CDosCommand::ReadOutputLoop() 
{
    char buffer[256];
    DWORD read;
    std::string line;

    dosCommandExitCode = EXIT_FAILURE;
    while (running) {
        BOOL success = ReadFile(hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &read, nullptr);
        if (!success || read == 0) break;
        buffer[read] = '\0';
        line.append(buffer);

        size_t pos;
        while ((pos = line.find('\n')) != std::string::npos) {
            std::string outLine = line.substr(0, pos);
            if (onOutput) onOutput(outLine);
            line.erase(0, pos + 1);
        }
    }
    dosCommandExitCode = getExitCode();

    running = false;
    CloseHandle(hChildStd_OUT_Rd);
    CloseHandle(hChildStd_IN_Wr);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    isThreadExitRequested = true;
}

void CDosCommand::Stop(bool force) 
{
    if (running) 
    {
        running = false;
        if (force && piProcInfo.hProcess) 
        {
            TerminateProcess(piProcInfo.hProcess, 1);
        }
        if (outputThread.joinable()) outputThread.join();
    }
}

bool CDosCommand::IsRunning() const 
{
    return running;
}

bool CDosCommand::WaitForExit(int timeoutMs) 
{
    if (!piProcInfo.hProcess) return true;

    DWORD result = WaitForSingleObject(piProcInfo.hProcess, timeoutMs);

    return result == WAIT_OBJECT_0;
}

int CDosCommand::getExitCode() const
{
    if (!piProcInfo.hProcess || piProcInfo.hProcess == INVALID_HANDLE_VALUE) return -1;

    DWORD exitCode = 0;
    BOOL success = GetExitCodeProcess(piProcInfo.hProcess, &exitCode);
    if (!success)
    {
        DWORD errCode = GetLastError();
        std::cerr << "GetExitCodeProcess failed. Error Kodu : " << errCode << std::endl;
        return -1;
        
    }
    return static_cast<int>(exitCode);
}

int CDosCommand::GetExitCode() const
{
    return dosCommandExitCode;
}


