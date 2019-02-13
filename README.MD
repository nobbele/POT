## C Preprocessor and Optimizer Thing

Run `clang -O3 -S -mllvm --x86-asm-syntax=intel -o- MYFILE` to get the result.

If you have Powershell you can use
`(clang -O3 -S -mllvm --x86-asm-syntax=intel -o- MYFILE | Select -Index 10).Split(",")[1].Trim()` to get the result.

The result is the number thats in the selected cell at the end of the code.

### Examples:
example 1
```c
#include "POT.h"

POTTEMPLATE(
  INC INC INC INC INC
  BEGINLOOP
    RIGHT INC INC INC LEFT DEC
  ENDLOOP
  RIGHT
)
```
Gives the output:
```
        .text
        .intel_syntax noprefix
        .def     test;
        .scl    2;
        .type   32;
        .endef
        .globl  test                    # -- Begin function test
        .p2align        4, 0x90
test:                                   # @test
# %bb.0:
        mov     dword ptr [rip + out], 15
        xor     eax, eax
        ret
                                        # -- End function
``` 
When using `clang -O3 -S -mllvm --x86-asm-syntax=intel -o- test.c`.

You can see the "15" at the bottom which is the result.

### How It Works

The C code that gets produced for Example 1 looks like
```c
int test() { 
  char _cells[64] = {0};
  char *const cells = _cells; 
  int cell_index = 0; 
  
  ++cells[cell_index]; 
  ++cells[cell_index]; 
  ++cells[cell_index]; 
  ++cells[cell_index]; 
  ++cells[cell_index]; 
  for(;cells[cell_index];) { 
    ++cell_index; 
    ++cells[cell_index]; 
    ++cells[cell_index]; 
    ++cells[cell_index]; 
    --cell_index; 
    --cells[cell_index];
  }
  
  ++cell_index; 
  out = cells[cell_index]; 
  return 0;
}
```
When formatted, if you know C you should understand how this works.
This will get compiled into assembly which looks like
https://pastebin.com/d6vDBAEy
But the magic happens after the optimizations, this code will become
https://pastebin.com/zQPNPmSv
And there is the result, 15.
