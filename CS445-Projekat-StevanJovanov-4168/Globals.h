#ifndef _GLOBALS_H
#define _GLOBALS_H

#define _CRT_SECURE_NO_WARNINGS

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#include <Windows.h>
#include <irrlicht.h>
#include <irrKlang.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <string>

#include "Utils.h"

#pragma comment(lib, "irrKlang.lib")

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif