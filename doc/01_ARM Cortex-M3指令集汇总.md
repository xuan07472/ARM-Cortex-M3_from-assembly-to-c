# ARM Cortex-M3指令集汇总

|作者|将狼才鲸|
|---|---|
|创建日期|2022-11-06|

* Cortex-M3不使用ARM指令集，而是使用Thumb或者Thumb-2指令集。
* M3共有116条指令（不是指令码的数量，一条指令会有多条指令码与之对应）。

* 指令集ARM官网在线阅读地址为[Processor instructions](https://developer.arm.com/documentation/100165/0201/Programmers-Model/Instruction-set-summary/Processor-instructions)。
* 在M3用户手册[ARM Cortex-M3 Processor Technical Reference Manual](https://developer.arm.com/documentation/100165/0201/)的表3-1中也有同样的描述。

<center>Table 3-1 Cortex-M3 instruction set summary</center>

| Operation | Description | Assembler | Cycles |
| --- | --- | --- | --- |
| Move| Register| MOV Rd, <op2>| 1|
|            | 16-bit immediate         | MOVW Rd, #<imm>             | 1|
|            | Immediate into top       | MOVT Rd, #<imm>             | 1|
|            | To PC                    | MOV PC, Rm                  | 1 + P|
| Add                      | Add                         | ADD Rd, Rn, <op2>           | 1|
|            | Add to PC                | ADD PC, PC, Rm              | 1 + P|
|            | Add with carry           | ADC Rd, Rn, <op2>           | 1|
|| Form address             | ADR Rd, <label>             | 1|
| Subtract                 | Subtract                    | SUB Rd, Rn, <op2>           | 1          |
| |Subtract with borrow     | SBC Rd, Rn, <op2>           | 1                           |
| |Reverse                  | RSB Rd, Rn, <op2>           | 1                           |
| Multiply                 | Multiply                    | MUL Rd, Rn, Rm              | 1          |
| |Multiply accumulate      | MLA Rd, Rn, Rm              | 2                           |
| |Multiply subtract        | MLS Rd, Rn, Rm              | 2                           |
| |Long signed              | SMULL RdLo, RdHi, Rn, Rm    | 3 to 5                      |
| |Long unsigned            | UMULL RdLo, RdHi, Rn, Rm    | 3 to 5                      |
| |Long signed accumulate   | SMLAL RdLo, RdHi, Rn, Rm    | 4 to 7                      |
| |Long unsigned accumulate | UMLAL RdLo, RdHi, Rn, Rm    | 4 to 7                      |
| |Divide                   | Signed                      | SDIV Rd, Rn, Rm             | 2 to 12    |
| Unsigned                 | UDIV Rd, Rn, Rm             | 2 to 12                     |
| |Saturate                 | Signed                      | SSAT Rd, #<imm>, <op2>      | 1          |
| |Unsigned                 | USAT Rd, #<imm>, <op2>      | 1                           |
| Compare                  | Compare                     | CMP Rn, <op2>               | 1          |
| |Negative                 | CMN Rn, <op2>               | 1                           |
| Logical                  | AND                         | AND Rd, Rn, <op2>           | 1          |
| |Exclusive OR             | EOR Rd, Rn, <op2>           | 1                           |
| |OR                       | ORR Rd, Rn, <op2>           | 1                           |
| |OR NOT                   | ORN Rd, Rn, <op2>           | 1                           |
| |Bit clear                | BIC Rd, Rn, <op2>           | 1                           |
| |Move NOT                 | MVN Rd, <op2>               | 1                           |
| |AND test                 | TST Rn, <op2>               | 1                           |
| |Exclusive OR test        | TEQ Rn, <op1>               |                             |
| Shift                    | Logical shift left          | LSL Rd, Rn, #<imm>          | 1          |
| |Logical shift left       | LSL Rd, Rn, Rs              | 1                           |
| |Logical shift right      | LSR Rd, Rn, #<imm>          | 1                           |
| |Logical shift right      | LSR Rd, Rn, Rs              | 1                           |
| |Arithmetic shift right   | ASR Rd, Rn, #<imm>          | 1                           |
| |Arithmetic shift right   | ASR Rd, Rn, Rs              | 1                           |
| Rotate                   | Rotate right                | ROR Rd, Rn, #<imm>          | 1          |
| |Rotate right             | ROR Rd, Rn, Rs              | 1                           |
| |With extension           | RRX Rd, Rn                  | 1                           |
| Count                    | Leading zeroes              | CLZ Rd, Rn                  | 1          |
| Load                     | Word                        | LDR Rd, [Rn, <op2>]         | 2          |
| |To PC                    | LDR PC, [Rn, <op2>]         | 2 + P                       |
| |Halfword                 | LDRH Rd, [Rn, <op2>]        | 2                           |
| |Byte                     | LDRB Rd, [Rn, <op2>]        | 2                           |
| |Signed halfword          | LDRSH Rd, [Rn, <op2>]       | 2                           |
| |Signed byte              | LDRSB Rd, [Rn, <op2>]       | 2                           |
| |User word                | LDRT Rd, [Rn, #<imm>]       | 2                           |
| |User halfword            | LDRHT Rd, [Rn, #<imm>]      | 2                           |
| |User byte                | LDRBT Rd, [Rn, #<imm>]      | 2                           |
| |User signed halfword     | LDRSHT Rd, [Rn, #<imm>]     | 2                           |
| |User signed byte         | LDRSBT Rd, [Rn, #<imm>]     | 2                           |
| |PC relative              | LDR Rd,[PC, #<imm>]         | 2                           |
| |Doubleword               | LDRD Rd, Rd, [Rn, #<imm>]   | 1 + N                       |
| |Multiple                 | LDM Rn, {<reglist>}         | 1 + N                       |
| |Multiple including PC    | LDM Rn, {<reglist>, PC}     | 1 + N + P                   |
| Store                    | Word                        | STR Rd, [Rn, <op2>]         | 2          |
| |Halfword                 | STRH Rd, [Rn, <op2>]        | 2                           |
| |Byte                     | STRB Rd, [Rn, <op2>]        | 2                           |
| |Signed halfword          | STRSH Rd, [Rn, <op2>]       | 2                           |
| |Signed byte              | STRSB Rd, [Rn, <op2>]       | 2                           |
| |User word                | STRT Rd, [Rn, #<imm>]       | 2                           |
| |User halfword            | STRHT Rd, [Rn, #<imm>]      | 2                           |
| |User byte                | STRBT Rd, [Rn, #<imm>]      | 2                           |
| |User signed halfword     | STRSHT Rd, [Rn, #<imm>]     | 2                           |
| |User signed byte         | STRSBT Rd, [Rn, #<imm>]     | 2                           |
| |Doubleword               | STRD Rd, Rd, [Rn, #<imm>]   | 1 + N                       |
| |Multiple                 | STM Rn, {<reglist>}         | 1 + N                       |
| |Push                     | Push                        | PUSH {<reglist>}            | 1 + N      |
| Push with link register  | PUSH {<reglist>, LR}        | 1 + N                       |
| |Pop                      | Pop                         | POP {<reglist>}             | 1 + N      |
| |Pop and return           | POP {<reglist>, PC}         | 1 + N + P                   |
| Semaphore                | Load exclusive              | LDREX Rd, [Rn, #<imm>]      | 2          |
| |Load exclusive half      | LDREXH Rd, [Rn]             | 2                           |
| |Load exclusive byte      | LDREXB Rd, [Rn]             | 2                           |
| |Store exclusive          | STREX Rd, Rt, [Rn, #<imm>]  | 2                           |
| |Store exclusive half     | STREXH Rd, Rt, [Rn]         | 2                           |
| |Store exclusive byte     | STREXB Rd, Rt, [Rn]         | 2                           |
| |Clear exclusive monitor  | CLREX                       | 1                           |
| Branch                   | Conditional                 | B<cc> <label>               | 1 or 1 + P |
| |Unconditional            | B <label>                   | 1 + P                       |
| |With link                | BL <label>                  | 1 + P                       |
| |With exchange            | BX Rm                       | 1 + P                       |
| |With link and exchange   | BLX Rm                      | 1 + P                       |
| |Branch if zero           | CBZ Rn, <label>             | 1 or 1 + P                  |
| |Branch if non-zero       | CBNZ Rn, <label>            | 1 or 1 + P                  |
| |Byte table branch        | TBB [Rn, Rm]                | 2 + P                       |
| |Halfword table branch    | TBH [Rn, Rm, LSL#1]         | 2 + P                       |
| State change             | Supervisor call             | SVC #<imm>                  | -          |
| |If-then-else             | IT... <cond>                | 1                           |
| |Disable interrupts       | CPSID <flags>               | 1 or 2                      |
| |Enable interrupts        | CPSIE <flags>               | 1 or 2                      |
| |Read special register    | MRS Rd, <specreg>           | 1 or 2                      |
| |Write special register   | MSR <specreg>, Rn           | 1 or 2                      |
| |Breakpoint               | BKPT #<imm>                 | -                           |            
| Extend                   | Signed halfword to word     | SXTH Rd, <op2>              | 1          |
| |Signed byte to word      | SXTB Rd, <op2>              | 1                           |
| |Unsigned halfword        | UXTH Rd, <op2>              | 1                           |
| |Unsigned byte            | UXTB Rd, <op2>              | 1                           |
| Bit field                | Extract unsigned            | UBFX Rd, Rn, #<imm>, #<imm> | 1          |
| |Extract signed           | SBFX Rd, Rn, #<imm>, #<imm> | 1                           |
| |Clear                    | BFC Rd, Rn, #<imm>, #<imm>  | 1                           |
| |Insert                   | BFI Rd, Rn, #<imm>, #<imm>  | 1                           |
| Reverse                  | Bytes in word               | REV Rd, Rm                  | 1          |
| |Bytes in both halfwords  | REV16 Rd, Rm                | 1                           |
| |Signed bottom halfword   | REVSH Rd, Rm                | 1                           |
| |Bits in word             | RBIT Rd, Rm                 | 1                           |            
| Hint                     | Send event                  | SEV                         | 1          |
| |Wait for event           | WFE                         | 1 + W                       |
|| Wait for interrupt       | WFI                         | 1 + W                       |
|| No operation             | NOP                         | 1                           |
| Barriers                 | Instruction synchronization | ISB                         | 1 + B      |
|| Data memory              | DMB                         | 1 + B                       |
|| Data synchronization     | DSB <flags>                 | 1 + B                       |