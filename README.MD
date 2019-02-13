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
<details>
<summary>Show assembly output</summary>
```x86asm
        .text
        .intel_syntax noprefix
        .def     test;
        .scl    2;
        .type   32;
        .endef
        .globl  test                    # -- Begin function test
        .p2align        4, 0x90
test:                                   # @test
.seh_proc test
# %bb.0:
        sub     rsp, 136
        .seh_stackalloc 136
        .seh_endprologue
        lea     rax, [rsp + 64]
        xor     edx, edx
        mov     ecx, 64
        mov     r8d, ecx
        mov     r9, rax
        mov     rcx, r9
        mov     qword ptr [rsp + 40], rax # 8-byte Spill
        call    memset
        mov     rax, qword ptr [rsp + 40] # 8-byte Reload
        mov     qword ptr [rsp + 56], rax
        mov     dword ptr [rsp + 52], 0
        mov     rcx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        mov     r10b, byte ptr [rcx + r8]
        add     r10b, 1
        mov     byte ptr [rcx + r8], r10b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        mov     r10b, byte ptr [rcx + r8]
        add     r10b, 1
        mov     byte ptr [rcx + r8], r10b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        mov     r10b, byte ptr [rcx + r8]
        add     r10b, 1
        mov     byte ptr [rcx + r8], r10b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        mov     r10b, byte ptr [rcx + r8]
        add     r10b, 1
        mov     byte ptr [rcx + r8], r10b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        mov     r10b, byte ptr [rcx + r8]
        add     r10b, 1
        mov     byte ptr [rcx + r8], r10b
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
        mov     rax, qword ptr [rsp + 56]
        movsxd  rcx, dword ptr [rsp + 52]
        cmp     byte ptr [rax + rcx], 0
        je      .LBB0_3
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
        mov     eax, dword ptr [rsp + 52]
        add     eax, 1
        mov     dword ptr [rsp + 52], eax
        mov     rcx, qword ptr [rsp + 56]
        movsxd  rdx, dword ptr [rsp + 52]
        mov     r8b, byte ptr [rcx + rdx]
        add     r8b, 1
        mov     byte ptr [rcx + rdx], r8b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  rdx, dword ptr [rsp + 52]
        mov     r8b, byte ptr [rcx + rdx]
        add     r8b, 1
        mov     byte ptr [rcx + rdx], r8b
        mov     rcx, qword ptr [rsp + 56]
        movsxd  rdx, dword ptr [rsp + 52]
        mov     r8b, byte ptr [rcx + rdx]
        add     r8b, 1
        mov     byte ptr [rcx + rdx], r8b
        mov     eax, dword ptr [rsp + 52]
        add     eax, -1
        mov     dword ptr [rsp + 52], eax
        mov     rcx, qword ptr [rsp + 56]
        movsxd  rdx, dword ptr [rsp + 52]
        mov     r8b, byte ptr [rcx + rdx]
        add     r8b, -1
        mov     byte ptr [rcx + rdx], r8b
        jmp     .LBB0_1
.LBB0_3:
        xor     eax, eax
        mov     ecx, dword ptr [rsp + 52]
        add     ecx, 1
        mov     dword ptr [rsp + 52], ecx
        mov     rdx, qword ptr [rsp + 56]
        movsxd  r8, dword ptr [rsp + 52]
        movsx   ecx, byte ptr [rdx + r8]
        mov     dword ptr [rip + out], ecx
        add     rsp, 136
        ret
        .seh_handlerdata
        .text
        .seh_endproc
                                        # -- End function
```
</details>
But the magic happens after the optimizations, this code will become
```x86asm
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
And there is the result, 15.
