/* Kernel 'Pages' Specific to GigaCompute OS */
/* Copyright 2018 Kenneth Vorseth            */
#ifndef PAGES_H
#define PAGES_H
#include <stdbool.h>  // <- FreeStanding C Headers
#include <stddef.h>   //
#include <stdint.h>   //
#include "io.h"       //< Provides Hardware-Level I/O Port Operations Required
#include "keyboard.h" //< Provides Keyboard Input and KeyPress Abstraction
#include "vga.h"		 //< Provides VGA Text-Mode Driver for Display
#include "ps2.h"		 //< Provides PS/2 Controller Driver
#include "utility.h"  //< Provides Utilities for FreeStanding Applications
#include "cmd.h"		 //< Provides CMD-LINE Utilities and Built-In Programs

static void POST_Screen(void)
{
	/*-----------------------------------------------------------------------------*/
	/* Initialize VGA Interface */
	VGA_Init(WHITE, BLACK);
	VGA_DrawStrColored("GigaCompute OS", 6,2, BLUE, BLACK);
	VGA_DrawStr("By Kenneth Vorseth | 2018", 6,3);
	VGA_DrawStr("[Bootloader]", 6,5);
	VGA_DrawStrColored("Bootloader", 7,5, BLUE, BLACK);
	VGA_DrawStr(" >Entering x86 Protected Mode", 6,6);
	VGA_DrawStr(" >Stack Aligned for High-Level Entry", 6,7);
	VGA_DrawStr(" >Kernel Launch Complete...", 6,8);
	/*-----------------------------------------------------------------------------*/
	/* Perform Power-On Self Test */
	VGA_DrawStr("[POST] Power-On Self Test", 6,10);
	VGA_DrawStrColored("POST", 7,10, BLUE, BLACK);
	/* >> PS/2 Controller */
	uint8_t psPost = PS2_POST();
	VGA_DrawStr(ToHex(psPost), 32, 11);
	VGA_DrawStr(" [X] PS/2 Controller", 6,11);
	//PS/2 Controller Passed Test
	if (IsBitClr(psPost, 0x04)) 
	{
		VGA_DrawColored('X', 8,11, GREEN, BLACK);
		
		//First Port is Active
		if (IsBitSet(psPost, 0x00))
		{
			//Display Device ID Type
			if (IsBitClr(psPost, 0x01)) 
			{ VGA_DrawStr(" [X] Port1 : Keyboard", 8,12); }
			else { VGA_DrawStr(" [X] Port1 : Mouse", 8,12); }
			VGA_DrawColored('X', 10,12, GREEN, BLACK);
		}
		else //First Port is Unactive
		{
			VGA_DrawStr(" [X] Port1 : n/a", 8,12);
			VGA_DrawColored('X', 10,12, LIGHT_GREY, BLACK);
		}
		
		//Second Port is Active
		if (IsBitSet(psPost, 0x02))
		{
			//Display Device ID Type
			if (IsBitClr(psPost, 0x03))
			{ VGA_DrawStr(" [X] Port2 : Keyboard", 8,13); }
			else { VGA_DrawStr(" [X] Port2 : Mouse", 8,13); }
			VGA_DrawColored('X', 10,13, GREEN, BLACK);
		}
		else //Second Port is Unactive
		{
			VGA_DrawStr(" [X] Port2 : n/a", 8,13);
			VGA_DrawColored('X', 10,13, LIGHT_GREY, BLACK);
		}
	}
	else //PS/2 Controller Failed Test
	{
		VGA_DrawColored('X', 8,11, RED, BLACK);
		VGA_DrawStr(" [X] Port1 : ?", 8,12);
		VGA_DrawColored('X', 10,12, RED, BLACK);
		VGA_DrawStr(" [X] Port2 : ?", 8,13);
		VGA_DrawColored('X', 10,13, RED, BLACK);
	}
	VGA_DrawStr("[Press 'Enter' to Continue]", 6,20);
	
	//Wait For Enter Key
	while (!Keyboard_GetKeyPress(key_enter)) { Keyboard_Update(); }
}

static void CMD_Screen(void)
{
	//Initialize CMD-LINE
	CMD_Refresh();
	
	while (1) //Main CMD-LINE Loop
	{
		//Update Current Program
		uint8_t retValue = (*programPtr)(0x0000,0x00);
		
		//Run Program [ENTER]
		if (Keyboard_GetKeyPress(key_enter)) { uint8_t status = CMD_Run(); }
		//Clear Line [Left Arrow]
		//...need to implement in scancodes (special make/break code)
		//Remove Last Character [BACKSPACE]
		if (Keyboard_GetKeyPress(key_backspace)) { CMD_Back(); } 
		
		//Update PS-2 Keyboard, Append Printable Characters to Screen
		char scanCode = Keyboard_Update();
		if ((uint8_t)scanCode != 0x00 && type_cursorX + 1 < VGA_WIDTH) 
		{ 
			//Apply Shift Modifier Key
			if (Keyboard_GetKey(key_shift)) { scanCode = shift_table[scanCode]; }
			
			VGA_WriteColored(scanCode, RED, BLACK); 	//Write Character to VGA Display
			cmdBuffer[type_cursorX] = scanCode;		//Write Character to VGA CMD-LINE Buffer
			type_cursorX++;							//Increment Cursor Position (Buffer Index)
		}
	}
}

#endif
