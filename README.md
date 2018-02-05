# Logic-system-class
IDE: CodeBlock(C++)

1.Purpose: 
To have a basic understanding in Central Processing Unit (CPU) by designing a preliminary CPU. 

2.Specification of CPU:
(1) 8-bit address
(2) 4 general purpose registers and 1 program counter (all 8-bit)
(3) Single-cycle (1 instruction is resolved in 1 cycle)

3.Instruction Set Architecture (ISA):
imm is short for immediate, Sel is short for select, Addr is short for address.
See Note.jpg for detailed explanation of every instruction.

4.I/O Specification:
(1) CPU should be compiled as executable file and read a file called input.txt without asking users to input the file’s name.
(2) input.txt contains the instructions being executed in binary format with no mark.
(3) CPU should output a file called output.txt after it finishes all operations.
(4) output.txt contains the content of data memory, registers and program counter in binary and decimal (2’s complement except program counter).
