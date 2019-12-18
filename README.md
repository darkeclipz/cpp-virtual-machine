# Virtual Machine C++

A virtual machine / CPU emulator written in C++. It contains a VM, assembler, and debugger.

![VM screenshot](vm.png)

# CPU

The CPU contains four general purpose registers, named `AX`, `BX`, `CX`, and `DX`. There is a stack pointer `SP` which indicates the top of the stack. There is a base pointer `BP` which is the bottom of the stack. The stack grows upwards, to lower memory. Finally there is the instruction pointer `ip`.

|Name | Value | Bits | Description |
| -- | -- |-- | -- |
|`AX` | 1 | 16 | Register A | 
|`BX` | 2 | 16 | Register B | 
|`CX` | 4 | 16 | Register C | 
|`DX` | 8 | 16 | Register D | 
|`SP` | 16 | 16 | Stack pointer | 
|`SB` | 32 | 16 | Stack base | 
|`BP` | 64 | 16 | Base pointer (consider replacing with flags) | 
|`IP` | 128 | 16 | Instruction pointer | 

The following flags are used in the virtual machine.

|Name | Value | Description |
| -- | -- | -- |
|`C` | 1  | Carry | 
|`Z` | 2  | Zero | 
|`I` | 4  | Interrupt | 
|`B` | 8  | Break | 
|`O` | 16  | Overflow | 
|`E` | 32  | Equal | 
|`L` | 64  | Less than | 
|`G` | 128  | Greater than | 


# Memory

Memory has the following layout:

| `0x00 - 0x1ffff` | `0x2000 + 0x24B0`  | `0x24B1 - sp` | `sp - 0x7FFF` | `0x8000 - 0xFFFF` |
| -- | -- | -- | -- | -- | 
| Empty | VGA | Empty | Stack | _Unused_ |

 * In the constructor of the virtual machine the amount of memory is currently set to 8kb.
 * The address range is `0x0000 - 0x8000`.
 * The VGA is mapped to address `0x2000` in memory. 
 * At the moment, a ROM is loaded at address `0x0000`.
 * The stack is located at the end of memory.

# Instructions

The virtual machine support the following opcodes.

See `opcode_table.xlsx`.

# Assembly

Explain the Assembly language.

## CALL/RET

The opcodes `CALL` and `RET` call another function. These functions need to do some housekeeping.

```
my_function:

    ...

    RET

CALL my_function
```

Should be translated into:

```
my_function:
    ...
    POP ip


PUSH ip
JMP my_function
```

## Including files

Before assembling, other assembly files should be included. They are annotated with the `@include` directive.

```
%include("file.asm)
```

# Debugger

 * Press `F5` to halt.
 * Enter `run` to start.
 * Enter `reset` to reset.
 * Enter `step` to execute one instruction.

# Todo

 * Write an assembler in Python.
   * Allow including of `asm` files with `%include("file.asm")`.
   * Implement labels for reference, defined with `label:`, and address is placed for `label`.
 * Generate better disassembler argument formatting.
 * Load a binary from a file.
 * Debugger command: load_rom, to load a binary into the VM.
 * Debugger command: reload, to reload the last load rom, for easier dev
 * Debugger command: reg = value, set the value of a register.
 * Debugger command: [mem] = value, set the value of a memory address.
 * Debugger breakpoints.
 * Expand the instruction set:
   * Arithmetic: `ADD`, `SUB`, `MUL`, `DIV`.
   * Logical operators: `AND`, `OR,` `NOT`, `XOR`, `SHL`, `SHR`.
   * Stack: `POP`, `PUSH`.
   * Functions: `CALL`, `RET`, but these can be rewritten in terms of `POP`, `PUSH`, and `JMP`.
 * Interrupts:
   * Keyboard input?
