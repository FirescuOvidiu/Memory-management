// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <crtdbg.h>

#include <vector>
#include <string>

// TODO: reference additional headers your program requires here

// Headers needed for WorstFit and PoolElement
#include <iostream>
#include <list>
#include <set>

// Headers needed for diagnostic tools
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <memory>

// Headers created
#include "PoolElement.h"
#include "Logger.h"
#include "Strategy.h"
#include "WorstFit.h"
#include "BestFit.h"
#include "BuddySystem.h"
#include "DiagnosticTools.h"
#include "DiagnoseExternalFragmentation.h"
#include "DiagnoseInternalFragmentation.h"
#include "MemoryManagement.h"