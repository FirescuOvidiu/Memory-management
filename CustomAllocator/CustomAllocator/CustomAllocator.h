
#ifndef _CUSTOM_ALLOCATOR_H_
#define _CUSTOM_ALLOCATOR_H_

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

//----------------------------------------------------------------------------

void * __cdecl CustomAllocator_New   (size_t aSize,  int aBlockUse = _NORMAL_BLOCK, char const * aFileName = __FILE__, int aLineNumber = 0);
void   __cdecl CustomAllocator_Delete(void * aBlock, int aBlockUse = _NORMAL_BLOCK, char const * aFileName = __FILE__, int aLineNumber = 0) noexcept;

void * __cdecl CustomAllocator_Malloc(size_t aSize,  int aBlockUse, char const * aFileName, int aLineNumber);
void __cdecl   CustomAllocator_Free  (void * aBlock, int aBlockUse, char const * aFileName, int aLineNumber);

void evaluateFragmentationState();

//----------------------------------------------------------------------------

#endif  // _CUSTOM_ALLOCATOR_H_
