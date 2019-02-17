//  Copyright 2019 Benjamin Bader
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "AlignedAllocations.h"

#include <new>

#if defined(_WIN32)

#include <malloc.h>

void* cppmetrics::AlignedAllocations::Allocate(std::size_t sz, std::size_t align)
{
  void* ptr = _aligned_malloc(sz, align);
  if (ptr == nullptr)
  {
    throw std::bad_alloc();
  }
  return ptr;
}

void cppmetrics::AlignedAllocations::Free(void* ptr)
{
  _aligned_free(ptr);
}

#else

#include <stdlib.h>

void* cppmetrics::AlignedAllocations::Allocate(std::size_t sz, std::size_t align)
{
  void* ptr = nullptr;
  if (posix_memalign(&ptr, align, sz) != 0)
  {
    throw std::bad_alloc();
  }
  return ptr;
}

void cppmetrics::AlignedAllocations::Free(void* ptr)
{
  free(ptr);
}

#endif
