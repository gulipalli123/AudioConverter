// WavToMp3.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "threadpool.h"
#include "lame/lame.h"
#include <vector>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <fstream>

// TODO: Reference additional headers your program requires here.
using namespace std;

static vector<string>
readDirectory(const string& directoryLocation, const string& extension);
bool LameEncode(const string& input);