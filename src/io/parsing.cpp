#include "module.h"

bool contains(const string& input, char c)
{
    int idx = input.find_first_of(c);
    return idx != string::npos;
}

bool starts_with(const string& input, char c)
{
    if (input.length() <= 0) return false;
    return input[0] == c;
}

bool case_insensitive_compare(char a, char b)
{
    return tolower(a) == tolower(b);
}

bool contains(const string& input, const string& substring)
{
    return search(input.begin(),
                  input.end(),
                  substring.begin(),
                  substring.end(),
                  case_insensitive_compare) != input.end();
}

bool starts_with(const string& input,
                 const string& startSequence,
                 bool caseSensitive)
{
    if (caseSensitive)
    {
        int cmpResult = input.compare(0, startSequence.length(), startSequence);
        return cmpResult == 0;
    }
    else
    {
        return equal(startSequence.begin(),
                     startSequence.end(),
                     input.begin(),
                     case_insensitive_compare);
    }
}

bool ends_with(const string& input, const string& substring, bool caseSensitive)
{
    int inStartIdx = input.length() - substring.length();
    if (inStartIdx < 0) return false;

    int subIdx = 0;
    for (int i = inStartIdx; i < input.length(); i++)
    {
        char cIn = input[i];
        char cSub = substring[subIdx++];

        if (caseSensitive)
        {
            if (!case_insensitive_compare(cIn, cSub)) return false;
        }
        else
        {
            if (cIn != cSub) return false;
        }
    }

    return true;
}

/**
 * Trims the basic whitespace characters: space, \t \n \r
 * Can be extended to trim more with the second parameter
 */
void trim(string& input, string additionalTrimChars = "")
{
    const string trimCharacters = additionalTrimChars.append(" \t\n\r");

    input.erase(0, input.find_first_not_of(trimCharacters));
    input.erase(input.find_last_not_of(trimCharacters) + 1);
}

/**
 * removes the starting sequence from the string if it exists
 */
void trim_start(string& input, const string& sequence)
{
    if (starts_with(input, sequence))
    {
        input.erase(0, sequence.length());
    }
}

void remove_until(string& input, char c, bool last)
{
    int idx;
    if (last)
        idx = input.find_last_of(c);
    else
        idx = input.find_first_of(c);

    if (idx != string::npos)
    {
        // we want to erase the last 'c' as well
        input.erase(0, idx + 1);
    }
}
