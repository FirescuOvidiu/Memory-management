
#include "stdafx.h"
#include "UseCustomAllocator.h"

//----------------------------------------------------------------------------
// NEW and DELETE operators:

void * __cdecl operator new(size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
  return CustomAllocator_New(aSize, aBlockUse, aFileName, aLineNumber);
}

void * __cdecl operator new[](size_t aSize, int aBlockUse, char const * aFileName, int aLineNumber)
{
  return CustomAllocator_New(aSize, aBlockUse, aFileName, aLineNumber);
}

void * __cdecl operator new(size_t aSize)
{
  return CustomAllocator_New(aSize);
}

void * __cdecl operator new(size_t aSize, std::nothrow_t const &) noexcept
{
  return CustomAllocator_New(aSize);
}

void * __cdecl operator new[](size_t aSize)
{
  return CustomAllocator_New(aSize);
}

void * __cdecl operator new[](size_t aSize, std::nothrow_t const &) noexcept
{
  return CustomAllocator_New(aSize);
}

void __cdecl operator delete(void * aBlock, int aBlockUse, char const * aFileName, int aLineNumber) noexcept
{
  return CustomAllocator_Delete(aBlock, aBlockUse, aFileName, aLineNumber);
}

void __cdecl operator delete[](void * aBlock, int aBlockUse, char const * aFileName, int aLineNumber) noexcept
{
  return CustomAllocator_Delete(aBlock, aBlockUse, aFileName, aLineNumber);
}

void __cdecl operator delete(void * aBlock) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

void __cdecl operator delete(void * aBlock, std::nothrow_t const &) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

void __cdecl operator delete[](void * aBlock) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

void __cdecl operator delete[](void * aBlock, std::nothrow_t const &) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

void __cdecl operator delete(void * aBlock, size_t /*aSize*/) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

void __cdecl operator delete[](void * aBlock, size_t /*aSize*/) noexcept
{
  return CustomAllocator_Delete(aBlock);
}

//----------------------------------------------------------------------------
