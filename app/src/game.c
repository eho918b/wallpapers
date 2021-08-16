/* 
 	SC2 Game Launcher - Windows version
	Linux Edition
	Eric Ho, 26-Jun-21
*/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlobj.h>
#define CSIDL_PERSONAL 0x0005        
#define SHGFP_TYPE_CURRENT 0
#include "config.h"

// config
extern const config_t config;

int game_launch()
{
    // find sc2 execution path
    unsigned int csidl = CSIDL_PERSONAL;
    char info_path[MAX_PATH] = { 0 };
    char sc2_path[MAX_PATH] = { 0 };
    HRESULT result = SHGetFolderPathA
    (
        NULL,
        csidl,
        NULL,
        SHGFP_TYPE_CURRENT,
        info_path
    );
    
    if (result == S_OK)
    {
        sprintf_s(info_path + strlen(info_path), MAX_PATH - strlen(info_path), "\\StarCraft II\\ExecuteInfo.txt");
        info_path[strlen(info_path)] = 0;
        FILE* fp;
        
        if (fopen_s(&fp, info_path, "r") != 0)
        {
            printf("[ERROR] ExecuteInfo.txt not found or wrong format...");
            exit(0);
        }
        else
        {
            printf("[client] info path: %s\n", info_path);
            while (fgets(info_path, sizeof(info_path) / sizeof(info_path[0]), fp) != NULL)
            {
                if (strncmp(info_path, "executable", 10) == 0)
                {
                    strcpy_s(sc2_path, sizeof(sc2_path) / sizeof(sc2_path[0]), info_path + 13);
                    sc2_path[strlen(sc2_path) - 1] = 0;
                }
            }
        }
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "[client] SHGetFolderPathA failed\n");
        return -1;
    }
    printf("[client] sc2 path:  %s\n", sc2_path);

	// launch process
    char current_directory[256] = {0};
    char command_line[256] = {0};  

    // command line
    // -listen 172.28.16.1 -port 5679 -displayMode 0 -windowwidth 2000 -windowheight 1000 -windowx 550 -windowy 0 -dataVersion;
    sprintf_s(command_line, sizeof(command_line) / sizeof(command_line[0]), " -listen %s -port %d -displayMode %d -windowwidth %d -windowheight %d -windowx %d -windowy %d",
        config.game.host, config.game.port, 0, config.game.window_width, config.game.window_height, config.game.window_x, config.game.window_y);

    // change directory
    //C:\Program Files(x86)\StarCraft II\Versions\Base81102\SC2_x64.exe
    //C:\Program Files(x86)\StarCraft II\Support64
    char* ptr = strstr(sc2_path, "StarCraft II");
    if (ptr == NULL)
    {
        printf("[ERROR] Support Directory issue");
        return 0;
    }
    __int64 offset = ptr - sc2_path;
    strncpy_s(current_directory, sizeof(current_directory) / sizeof(current_directory[0]), sc2_path, offset);
    sprintf_s(current_directory + offset, sizeof(current_directory) / sizeof(current_directory[0]) - offset, "%s", "StarCraft II\\Support64");
    if (SetCurrentDirectoryA(current_directory) == 0)
    {
        printf("[ERROR] cannot change current directory\n");
        return -1;
    }

    // prepare
    printf("[Current Directory] %s\n", current_directory);
    printf("[Process Path]      %s\n", sc2_path);
    printf("[Command Line]     %s\n", command_line);

    // create process
    struct WindowsProcess
    {
        STARTUPINFOA si_;
        PROCESS_INFORMATION pi_;
    } process;
    memset(&process, 0, sizeof(process));
    process.si_.cb = sizeof(process.si_);
    if (!CreateProcessA((LPCSTR)sc2_path, // Module name
        command_line,           // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &process.si_,           // Pointer to STARTUPINFO structure
        &process.pi_))          // Pointer to PROCESS_INFORMATION structure
    {                           
        printf("[ERROR] cannot create process\n");
        return -1;
    }
    
    // special -> trick to accelerate startup process
    system("ipconfig /release 1>nul & ping -n 2 127.0.0.1 1>nul & ipconfig /renew 1>nul");
   
    // done
	return 0;
}
