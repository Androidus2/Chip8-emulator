#pragma once
#include "windows.h"
#include <commdlg.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Comdlg32.lib")

std::string OpenFileDialog(bool isROM);

std::string SaveFileDialog();