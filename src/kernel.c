/* 'GigaCompute OS' Kernal Entry  */
/* Copyright 2018 Kenneth Vorseth */

#include <stdbool.h>  	// <- FreeStanding C Headers
#include <stddef.h>   	//
#include <stdint.h>   	//
//#include "interrupts.h"	//< Provides GDT and Interrupt Setup
#include "io.h"       	//< Provides Hardware-Level I/O Port Operations Required
#include "keyboard.h" 	//< Provides Keyboard Input and KeyPress Abstraction
#include "vga.h"		 	//< Provides VGA Text-Mode Driver for Display
#include "ps2.h"		 	//< Provides PS/2 Controller Driver
#include "utility.h"  	//< Provides Utilities for FreeStanding Applications
#include "pages.h"	 	//< Defines Kernel User-Interface Screens

 
/* Ensure Proper Host Operating System (Windows) and Target (i386) */
#if defined(__linux__)
#error "Cross-Compiler Was Designed for Windows! Abort now!"
#endif
#if !defined(__i386__)
#error "i386-ELF Compiler Needed for Building Freestanding C Kernal"
#endif


void kernel_main(void) 
{
	//Run Post Screen
	POST_Screen();
	
	//Command Interpreter Screen
	CMD_Screen();
}