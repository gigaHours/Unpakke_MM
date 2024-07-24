#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mutex;
int countFinished;

void startup(std::string prog, std::string module, std::string tabFile, std::string output)
{
    // additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    auto line = (prog + " " + "\"" + module + "\"" + " unpack " + "\"" + tabFile + "\"" + " " + "\"" + output + "\"");
    //std::cout << line << std::endl;
    // start the program up
    CreateProcessA
    (
        prog.c_str(),   // the path
        (char*)line.c_str(),                // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,     // Opens file in a separate console
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi           // Pointer to PROCESS_INFORMATION structure
    );

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    mutex.lock();
    std::cout << "Processed " + tabFile << std::endl;
    countFinished += 1;
    mutex.unlock();
}

std::string dirnameOf(const std::string& fname)
{
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
        ? ""
        : fname.substr(0, pos);
}

inline bool ends_with(std::string const& value, std::string const& ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string GetCurrentDirectorya()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
}

void StartUnpack(std::string prog, std::string module, std::string tabFile, std::string output)
{
    startup(prog, module, tabFile, output);
}

int main(int argc, char** argv)
{
    std::cout << "UNPACKER BY ImmortanWork(gigaHours)" << std::endl;

    for (int i = 1; i < argc; i++)
    {
        auto file = std::string(argv[i]);
        if (!ends_with(file, ".tab")) continue;

        auto path = GetCurrentDirectorya() + "\\bin\\";
        auto module = path + "modules\\avalanche.umod";
        auto output = dirnameOf(argv[i]) + "\\unpacked";

        CreateDirectoryA(output.c_str(), 0);

        StartUnpack((GetCurrentDirectorya() + "\\bin\\unpakke.exe"), module, file, output);
    }

    char d[4];
    std::cout << "Enter something for close\n";
    scanf("%s", &d);

    return 0;
}
