#pragma once
#include <stdint.h>

namespace Kernel::CPU::Interrupts {    
    struct IDTEntry
    {
        uint16_t Offset0;
        uint16_t Selector;
        uint8_t Ist;
        uint8_t TypesAttrib;
        uint16_t Offset1;
        uint32_t Offset2;
        uint32_t Reserved;
    }__attribute__((packed));

    struct IDTR
    {
        uint16_t Limit;
        uint64_t Addr;
    }__attribute__((packed));
    
    struct Registers
    {
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t r11;
        uint64_t r10;
        uint64_t r9;
        uint64_t r8;
        uint64_t rbp;
        uint64_t rdi;
        uint64_t rsi;
        uint64_t rdx;
        uint64_t rcx;
        uint64_t rbx;
        uint64_t rax;
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    }__attribute__((packed));

    void Initialize();
}