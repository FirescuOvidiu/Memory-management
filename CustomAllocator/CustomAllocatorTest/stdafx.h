// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

// TODO: reference additional headers your program requires here

// Headers needed for CustomAllocatorTest
#include <new>
#include <random>
#include <fstream>
#include <set>
#include <array>

// Headers created
#include "CustomAllocatorTestA.h"
#include "CustomAllocatorTestB.h"
#include "CustomAllocatorTestC.h"
#include "CustomAllocatorTestD.h"
#include "GenerateTestUnits.h"
#include "AllocationObject.h"
#include "DeallocationObject.h"

// install our custom memory allocator
#include "../CustomAllocator/UseCustomAllocator.h"
