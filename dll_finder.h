#ifndef DLL_FINDER_H

#include <vector>
#include <iostream>
#include <fstream>

#define DLL_FINDER_H

bool FindTagsInDLL(std::string value, std::string dllpath);
static std::vector<char> ReadAllBytes(char const* filename);


#endif // DLL_FINDER_H
