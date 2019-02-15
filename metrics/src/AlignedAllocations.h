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

// Implements a variant of aligned_alloc from c++17,
// using platform-specific mechanisms.

#ifndef CPPMETRICS_METRICS_ALIGNEDALLOCATIONS_H
#define CPPMETRICS_METRICS_ALIGNEDALLOCATIONS_H

#include <cstddef>
#include <utility>

namespace cppmetrics { namespace AlignedAllocations {

void* Allocate(std::size_t sz, std::size_t align);
void Free(void* ptr);

template <typename T, std::size_t A = alignof(T), typename ...Args>
T* Make(Args&& ...args)
{
  void* ptr = Allocate(sizeof(T), A);
  return new (ptr) T(std::forward<Args>(args)...);
}

}}

#endif
