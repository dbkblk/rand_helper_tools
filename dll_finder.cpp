#include "dll_finder.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <QDebug>

using namespace std;

static std::vector<char> ReadAllBytes(char const* filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

bool FindTagsInDLL(std::string value, std::string dllpath)
{
    std::vector<char> v = ReadAllBytes(dllpath.c_str());
    int str_len = strlen(value.c_str());
    //qDebug() << "String lenght is " << str_len;

    char myString[1024];

    int iOffset = 0;

    for(std::vector<char>::iterator it = v.begin(); it != v.end(); ++it)
    {
        myString[iOffset] = *it;
        iOffset++;

        if (iOffset <= str_len) {
            if (value.compare(0, iOffset, myString, iOffset) != 0) {
                iOffset = 0;
            }

        }
        else
        {
            // it's a string, which starts with TXT_KEY_
            if (*it == 0) {
                iOffset = 0;
                //printf("%s\n", myString);
                return true;
            }
            else if (iOffset == 1023)
            {
                // string name is more than 1024 characters.... something is likely wrong here.
                iOffset = 0;
            }
        }
    }
    return false;
}
