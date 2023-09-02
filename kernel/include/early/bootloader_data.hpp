/*
    * bootloader_data.hpp
    * Declarations for bootloader data component
    * Created 01/09/23 DanielH
*/

#pragma once
#include <limine.h>

struct BootloaderData
{
    limine_framebuffer_response fbData;
    limine_memmap_response memmap;
    limine_smp_response smp;
};

BootloaderData GetBootloaderData();
