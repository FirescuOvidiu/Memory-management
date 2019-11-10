#include "stdafx.h"
#include "CustomAllocator.h"

//----------------------------------------------------------------------------

void * __cdecl CustomAllocator_New(size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
  return CustomAllocator_Malloc(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl CustomAllocator_Delete(void * aBlock, int aBlockUse, char const * aFileName, int aLineNumber) noexcept
{
  CustomAllocator_Free(aBlock, aBlockUse, aFileName, aLineNumber);
}

void * __cdecl CustomAllocator_Malloc(size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
  // default CRT implementation
  return _malloc_dbg(aSize, aBlockUse, aFileName, aLineNumber);
}

void __cdecl CustomAllocator_Free(void * aBlock, int aBlockUse, char const * /*aFileName*/, int /*aLineNumber*/)
{
  // default CRT implementation
  _free_dbg(aBlock, aBlockUse);
}