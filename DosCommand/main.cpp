#ifdef _WIN32
    #include <clocale>
    #include <windows.h>
#elif __linux__
    #include <locale>
#elif __APPLE__
    #include <locale>
#else
    #error ...
#endif

#include <iostream>
#include <chrono>
#include <thread>
#include "src/DosCommand.h"

void enableTurkishEncoding(void)
{
#ifdef _WIN32
    setlocale(LC_ALL, "Turkish");
#elif __linux__
    std::locale::global(std::locale("tr_TR.UTF - 8"));
#elif __APPLE__
    std::locale::global(std::locale("tr_TR.UTF - 8"));
#else

#endif
}

void myOutputCallback(const std::string& output)
{
    std::cout << ">> " << output << std::endl;
}

/*
    source klasöründeki tüm dosya ve klasörleri backup.zip ve backup.rar
    olarak destination klasörüne rar'lar

    std::string scriptName = "";
    std::string scriptPath = "";
    std::string cmdString = "";
    std::string cmdMessage = "";
    getCommandString(scriptName, scriptPath, cmdString, cmdMessage);
*/
void getCommandString(std::string& scriptName, std::string& scriptPath, std::string& cmdString, std::string& cmdMessage)
{
    scriptName              = "archiveFilesWithWinRar.bat";
    scriptPath              = "scripts\\" + scriptName;
    std::string destination = "C:\\Temp";
    std::string source      = "C:\\Temp";

    cmdString = scriptPath + " " + destination + " " + source;

    cmdMessage = scriptName + " baslatiliyor...";
}

/*
    cmdString = "ping 127.0.0.1 -n 10";

    std::cout << "ping komutu baslatiliyor...\n";
*/
void getCommandString2(std::string& cmdString, std::string& cmdMessage)
{
    cmdString = "ping 127.0.0.1 -n 10";

    cmdMessage = "ping komutu baslatiliyor...\n";
}

/*
    cmdString = "dir C:\\Temp";

    cmdMessage = "dir komutu baslatiliyor...";
*/
void getCommandString3(std::string& cmdString, std::string& cmdMessage)
{
    cmdString = "dir C:\\Temp";

    cmdMessage = "dir komutu baslatiliyor...";
}

/*
    cmdString = "ping localhost";

    cmdMessage = "ping komutu baslatiliyor...\n";
*/
void getCommandString4(std::string& cmdString, std::string& cmdMessage)
{
    cmdString = "ping localhost";

    cmdMessage = "ping komutu baslatiliyor...\n";
}

int main1()
{
    CDosCommand cmd;

    std::string scriptName = "";
    std::string scriptPath = "";
    std::string cmdString = "";
    std::string cmdMessage = "";
    getCommandString(scriptName, scriptPath, cmdString, cmdMessage);
    getCommandString2(cmdString, cmdMessage);
    getCommandString3(cmdString, cmdMessage);
    getCommandString4(cmdString, cmdMessage);

    // -------------------------------------------------------------------
    std::cout << std::endl;
    std::cout << cmdMessage << std::endl;
    std::cout << std::endl;
    // -------------------------------------------------------------------
    {
        bool started = false;
        started = cmd.Start(cmdString, [](const std::string& output)
            {
                std::cout << "  >>  " << output << std::endl;
            }
        );

        if (!started)
        {
            std::cerr << "Komut baslatilamadi!\n";
            return 1;
        }
    }

    // -------------------------------------------------------------------
    std::cout << std::endl;
    std::cout << "Komut bekleniyor.\n";
    std::cout << std::endl;
    // -------------------------------------------------------------------
    {
        // islemin bitmesini bekle
        if (!cmd.WaitForExit())
        {
            std::cerr << "Zaman Asimi! Komut hala çalisiyor. Zorla durduruluyor...\n";
            cmd.Stop(true);

            int result = cmd.GetExitCode();
            std::cout << "Exit Kodu (muhtemelen 1) : " << result << std::endl;

        }
        else
        {
            if (cmd.IsRunning())
            {
                std::cout << "Cikis tamamlanmis, temizleniyor...\n";
                cmd.Stop();
            }

            int result = cmd.GetExitCode();
            if (result == 0)
            {
                std::cout << std::endl;
                std::cout << "Komut basariyla tamamlandi" << std::endl;
                std::cout << std::endl;
            }
            else
            {
                std::cerr << "Komut hata ile bitti. Hata Kodu : " << result << std::endl;
            }
        }
    }

    // -------------------------------------------------------------------
    std::cout << std::endl;
    std::cout << "Program tamamlandi.\n";
    std::cout << std::endl;
    // -------------------------------------------------------------------

    return 0;
}

int main2()
{
    CDosCommand cmd;

    // -------------------------------------------------------------------
    std::cout << std::endl;
    std::cout << "sort komutu baslatiliyor...\n";
    std::cout << std::endl;
    // -------------------------------------------------------------------
    {
        bool started = false;
        started = cmd.Start("sort", [](const std::string& output)
            {
                std::cout << "  >>  " << output << std::endl;
            }
        );

        if (!started)
        {
            std::cerr << "Komut baslatilamadi!\n";
            return 1;
        }
    }

    // -------------------------------------------------------------------
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Komuta veri gönder
        std::cout << " elma gonderiliyor...." << std::endl;        
        cmd.WriteInput("elma");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        std::cout << " armut gonderiliyor..." << std::endl;
        cmd.WriteInput("armut");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::cout << " muz gonderiliyor....." << std::endl;
        cmd.WriteInput("muz");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::cout << " kiraz gonderiliyor..." << std::endl;
        cmd.WriteInput("kiraz");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // Ctrl+Z esdegeri olarak dosya sonu (EOF) vererek sort'u sonlandir
        std::cout << " Ctrl+Z gonderiliyor...." << std::endl;
        cmd.WriteInput("\x1A");  // ASCII 26 = Ctrl+Z

        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    // -------------------------------------------------------------------

    cmd.Stop();

    // -------------------------------------------------------------------
    std::cout << std::endl;
    std::cout << "Program tamamlandi.\n";
    std::cout << std::endl;
    // -------------------------------------------------------------------

    return 0;
}

int main()
{
    enableTurkishEncoding();

    return main2();
}

