// Integers.hpp
// Written by TechEverything

/*
Shorter types than the ones defined in <cstdint>
*/

#pragma once

// 8-bit
using i8  = __INT8_TYPE__;
using u8  = __UINT8_TYPE__;

// 16-bit
using i16 = __INT16_TYPE__;
using u16 = __UINT16_TYPE__;

// 32-bit
using i32 = __INT32_TYPE__;
using u32 = __UINT32_TYPE__;

// 64-bit
using i64 = __INT64_TYPE__;
using u64 = __UINT64_TYPE__;

// pointer
using uptr = __UINTPTR_TYPE__;
using size = uptr;