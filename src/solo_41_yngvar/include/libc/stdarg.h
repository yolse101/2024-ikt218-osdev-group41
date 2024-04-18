#pragma once

typedef __builtin_va_list va_list; // va_list, list of variable arguments used in print()
#define va_start(v, l) __builtin_va_start(v, l) // va_start
#define va_end(v) __builtin_va_end(v) // va_end
#define va_arg(v, l) __builtin_va_arg(v, l) // va_arg
