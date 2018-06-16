/* Defined Built-In CMD-LINE Executables */
/* Copyright 2018 Kenneth Vorseth        */
#ifndef CMD_LINE_H
#define CMD_LINE_H
#include <stdbool.h>  // <- FreeStanding C Headers
#include <stddef.h>   //
#include <stdint.h>   //
#include "io.h"       //< Provides Hardware-Level I/O Port Operations Required
#include "keyboard.h" //< Provides Keyboard Input and KeyPress Abstraction
#include "vga.h"		 //< Provides VGA Text-Mode Driver for Display
#include "ps2.h"		 //< Provides PS/2 Controller Driver
#include "utility.h"  //< Provides Utilities for FreeStanding Applications

/* CMD-LINE Executables */
char programs[2][12] =
{
	[0]="CMD-LINE",
	[1]="hexdump"
};

/* CMD-LINE Exe Functions */
static uint8_t cmdline(uint8_t* arg, uint8_t argLen)
{
	return 0;
}
static uint8_t hexdump(uint8_t* arg, uint8_t argLen)
{
	arg = &key_states;
	const uint8_t hdX = 24;
	const uint8_t hdY = 3;
	//LATER NEED TO ALIGN TO BIT-WISE BOUNDARY
	
	//Draw Top Table Column Header
	VGA_DrawStrColored("   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F", hdX, hdY, GREEN, BLACK);
	//Draw Row by Row
	for (uint8_t r = 0; r < 16; r++)
	{
		//Draw Row Header
		VGA_DrawStrColored(ToHexShort16(arg + r * 0x0F), hdX - 2, r + hdY + 1, GREEN, BLACK);
		//Draw Each Element
		for (uint8_t c = 0; c < 16; c++)
		{
			uint8_t color = (arg[c + (0x0F * r)] != 0x00) ? RED : BLUE;
			VGA_DrawStrColored(ToHexShort(arg[c + (0x0F * r)]), (c * 3) + hdX + 3, r + hdY + 1, color, BLACK);
		}
	}
	
	return 0;
}

/* Useful Functions for Text-Interface */
const uint8_t type_boundaryX = 2;
uint8_t type_cursorX = 0;
char cmdBuffer[80] = { ' ' };
char programName[12] = "CMD-LINE";
uint8_t (*programPtr)(uint16_t , uint8_t) = &cmdline;

static void CMD_Refresh() //[Refreshes Page, Draws Program Name Centered]
{
	//Clear Entire Screen
	VGA_Clear();

	//Draw Borders
	for (int x = 1; x < VGA_WIDTH - 1; x++)
	{
		VGA_DrawColored(' ', x, 0, BLUE, BLUE);
		VGA_DrawColored(' ', x, 1, BLACK, BLACK);
		VGA_DrawColored(' ', x, 1, LIGHT_GREY, BLACK);
		VGA_DrawColored(' ', x, VGA_HEIGHT - 3, BLUE, BLUE);
		VGA_DrawColored(' ', x, VGA_HEIGHT - 1, BLUE, BLUE);
	}
	for (int y = 0; y < VGA_HEIGHT; y++)
	{
		VGA_DrawColored(' ', 0, y, BLUE, BLUE);
		VGA_DrawColored(' ', VGA_WIDTH - 1, y, BLUE, BLUE);
	}
	//Draw Program Name and Header
	VGA_DrawStrColored("[GigaCompute OS]", VGA_WIDTH / 2 -  8, 0, BLUE, BLACK);
	//Re-Draw Program Name
	VGA_DrawColored('[', 1, 1, RED, BLUE);
	VGA_DrawStrColored(programName, 2, 1, RED, BLUE | BLINK);
	VGA_DrawColored(']', StrLength(programName) + 2, 1, RED, BLUE);
	//Draw Input Box Area
	VGA_DrawColored('>', 1, VGA_HEIGHT - 2, RED, BLACK);
	VGA_SetCursor(2, VGA_HEIGHT - 2);
	//Clear CommandLine Buffer
	for (int i = 0; i < 80; i++) { cmdBuffer[i] = ' '; }
	type_cursorX = 0;
}

static uint8_t CMD_Run() //[Attempts Running Program]
{
	//Search for Known Program
	uint8_t foundProgram = 0x00; //Index of Found Program (Zero=Not Found)
	for (uint8_t p = 0; p < 2; p++) //For Each Program
	{
		for (uint8_t l = 0; l < StrLength(programs[p]); l++) //For Each Letter in Program Name
		{
			//Check if Letter is NOT Program Name
			if (programs[p][l] != cmdBuffer[l]) { foundProgram = 0x00; break; }
			else { foundProgram = p; }
		}
		//Early Break if Program Found
		if (foundProgram != 0x00) { break; }
	}
	//Early Exit if Not Found
	if (foundProgram == 0x00) { return 0x00; }
	
	//Run Program if Found
	StrCopy(programs[foundProgram], programName);
	switch (foundProgram)
	{
		case 0x01:		//hexdump <16-bit>
			programPtr = &hexdump;
			break;
		default:		//cmd-line return
			StrCopy(programs[0x00], programName);
			programPtr = &cmdline;
			break;
	}
	//Refresh Screen
	CMD_Refresh();
	
	
	//Display Success of Failure of Finding Program
	return foundProgram;
}

static void CMD_Back() //[Attempts Moving Cursor Backwards]
{
	//Prevent Cursor From Leaving Left Boundary
	if (VGA_CURSOR_X > type_boundaryX)
	{
		VGA_Back();						//Remove Character from VGA Display
		cmdBuffer[type_cursorX] = ' '; 	//Remove Character from CMD-LINE Buffer
		type_cursorX--;					//Decrement Cursor Position
	}
}

#endif
