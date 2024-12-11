#include "module.h"

vector<FileEntry> directory_files(const string directory)
{
    vector<FileEntry> files;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        Logf("Error, directory was not found: %s", directory.c_str());
    }
    else
    {
        do
        {
            string name = findFileData.cFileName;
            if (name == "." || name == "..") continue;

            if (~findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                files.push_back({name, directory + "\\" + name});
            }
        }
        while (FindNextFileA(hFind, &findFileData) != 0);
    }

    return files;
}

bool VerifyFile(ifstream& file, string path)
{
    if (!file.is_open())
    {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        Logf("Did not find file '%s'. Current dir '%s'", path.c_str(), buffer);
        return false;
    }
    return true;
}

void Trim(string& str)
{
    str.erase(0, str.find_first_not_of(" \t"));
    str.erase(str.find_last_not_of(" \t") + 1);
}

const char COMMENT = '#';

wstring to_wstring(const string str)
{
    if (str.empty()) return wstring();
    int neededSize = MultiByteToWideChar(CP_UTF8,
                                         0,
                                         &str[0],
                                         (int)str.size(),
                                         NULL,
                                         0);
    wstring wstrTo(neededSize, 0);
    MultiByteToWideChar(CP_UTF8,
                        0,
                        &str[0],
                        (int)str.size(),
                        &wstrTo[0],
                        neededSize);
    return wstrTo;
}

bool file_exists(string filePath)
{
    ifstream file(filePath);
    return file.good();
}

string extract_file_name(string filePath)
{
    size_t pos = filePath.find_last_of("/\\");
    return (pos == string::npos) ? filePath : filePath.substr(pos + 1);
}

unordered_map<string, string> read_kvp_file(const string& filePath,
                                            const char delimiter)
{
    unordered_map<string, string> kvps;
    ifstream file(filePath);
    if (!VerifyFile(file, filePath)) return kvps;

    string line;
    while (getline(file, line))
    {
        if (line.empty() || line[0] == COMMENT) continue;

        size_t pos = line.find(delimiter);
        if (pos != string::npos)
        {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            Trim(key);
            Trim(value);

            kvps[key] = value;
        }
    }

    file.close();

    return kvps;
}
