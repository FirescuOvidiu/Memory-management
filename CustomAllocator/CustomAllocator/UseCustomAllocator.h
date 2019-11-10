
#ifndef _USE_CUSTOM_ALLOCATOR_H_
#define _USE_CUSTOM_ALLOCATOR_H_

#include "CustomAllocator.h"

//----------------------------------------------------------------------------

#ifdef _DEBUG

//#define MY_NEW          new (_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new             MY_NEW

#define MY_MALLOC(size)   CustomAllocator_Malloc(size,  _NORMAL_BLOCK, __FILE__, __LINE__)
#define MY_FREE(block)    CustomAllocator_Free  (block, _NORMAL_BLOCK, __FILE__, __LINE__)
#undef  malloc
#define malloc            MY_MALLOC
#undef  free
#define free              MY_FREE

#endif  // _DEBUG

//----------------------------------------------------------------------------


#endif  // _USE_CUSTOM_ALLOCATOR_H_
