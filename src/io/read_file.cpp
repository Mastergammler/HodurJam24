#include "module.h"
#include "utils.cpp"

vector<string> read_all_lines(const string filePath)
{
    vector<string> lines;

    ifstream file(filePath);
    if (!VerifyFile(file, filePath)) return lines;

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;
        lines.push_back(line);
    }

    return lines;
}
