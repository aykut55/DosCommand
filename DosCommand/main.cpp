#include <iostream>
#include "src/DosCommand.h"

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

int main()
{
    CDosCommand cmd;

    std::string scriptName = "";
    std::string scriptPath = "";
    std::string cmdString = "";
    std::string cmdMessage = "";
    getCommandString(scriptName, scriptPath, cmdString, cmdMessage);
    getCommandString2(cmdString, cmdMessage);

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
