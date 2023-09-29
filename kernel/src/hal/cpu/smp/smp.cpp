/*
    * smp.cpp
    * Symmetric multi-processing
    * Created 02/09/2023
*/

#include <limine.h>
#include <stddef.h>
#include <hal/cpu/gdt.hpp>
#include <hal/cpu/interrupt/idt.hpp>
#include <hal/cpu/interrupt/apic.hpp>
#include <hal/cpu.hpp>
#include <terminal/terminal.hpp>
#include <libs/kernel.hpp>
#include <hal/acpi.hpp>

limine_smp_info *smp_info = nullptr;
uint32_t num_cpu = 0; 

namespace Kernel::CPU {
    void SMPSetup(limine_smp_info* smp_info, uint32_t cpu_count) {
        ::smp_info = smp_info;
        num_cpu = cpu_count;
    }

    void CPUJump(unsigned int cpu, void* ptr) {
        if (!cpu) return; // CPU must be > 0
        if (!smp_info) return;

        smp_info[cpu].goto_address = (limine_goto_address)ptr;
    }

    void CPUShutdown() {
        CPU::ClearInterrupts();
        CPU::Halt();

        while (true) {
            CPU::Halt();
        }
    }

    void PerCPUSetup(limine_smp_info *) {
        CPU::GDT::Initialize();
        CPU::Interrupts::Install();
        CPU::InitializeLAPIC();

        while (true) {
            CPU::Halt();
        }
    }

    void SetupAllCPUs() {
        ACPI::InitializeACPI();
        Kernel::Log(KERNEL_LOG_EVENT, "Calibrating Local APIC timer\n");
        CalibrateTimer();
        ClearInterrupts();

        if (!smp_info) return;

        if (num_cpu == 1) {
            Kernel::Log(KERNEL_LOG_DEBUG, "Using single-processor setup.\n");
            // Only LAPIC initialization is required as GDT and IDT are already installed
            // on the BSP.
            CPU::InitializeLAPIC();
        } else {
            for (uint32_t i = 1; i < num_cpu; i++) {
                CPUJump(i, (void *)&PerCPUSetup);
            }
        }

        Kernel::CPU::InitializeIOAPIC(); // Setup I/O APIC globally.
        
        /*  Acknowledge and discard the last keyboard IRQ 
            in case the user hit a key before the IO APIC 
            was set up, which would result in keyboard 
            interrupts "not working".
        */
        IO::inb(0x60);
        SetInterrupts();
    }
}
