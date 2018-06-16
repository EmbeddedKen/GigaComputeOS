/* VGA Text-Mode Display Driver for OS Development */
/* Copyright 2018 Kenneth Vorseth                  */
#ifndef VGA_H
#define VGA_H
#include <stdbool.h> // <- FreeStanding C Headers
#include <stddef.h>	//
#include <stdint.h>	//
#include "io.h"      //< Provides Hardware-Level I/O Port Operations Required
#include "utility.h" //< Provides Utilities for FreeStanding Applications
 
/* VGA Basic Colors */
#define BLACK 			0x00
#define BLUE 			0x01
#define GREEN			0x02
#define CYAN				0x03
#define RED 				0x04
#define MAGENTA 			0x05
#define BROWN 			0x06
#define LIGHT_GREY 		0x07
#define WHITE 			0X0F
#define BLINK			0x08 //< To be OR'd into BG Color to Enable Blink
/* VGA Text-Mode Dimensions */
#define VGA_WIDTH		80
#define VGA_HEIGHT		25
/* VGA Cursor  */
uint8_t VGA_CURSOR_X;
uint8_t VGA_CURSOR_Y;
/* VGA Defaults */
uint8_t VGA_COLOR_BG;
uint8_t VGA_COLOR_FG;
/* VGA Plane Buffer */
uint16_t* VGA_BUFFER;

//Character Attribute Register (Index: 0x10)
//val &= ~(0x08); //Disable Blink
//val &= ~(0x04); //Continuity for 0xC0-DF Chars

/* Writes to a VGA Attribute Register */
/*
static void VGA_WriteAttribute(uint16_t index, uint8_t val)
{
	PORT_READ8(0x3DA); 							//Set Flip/Flop to Index
	uint8_t adr = PORT_READ8(0x3C0); 			//Read Address Register
	PORT_WRITE8(0x3C0, 0x20 | index); 			//Index of Register to Write
	PORT_READ8(0x3C1); 							//Read Data Register 
	PORT_WRITE8(0x3C0, val); 					//Write to Data Register 
	PORT_WRITE8(0x3C0, adr); 					//Write Address Register Back
	PORT_READ8(0x3DA); 							//Set Flip/Flop to Index
}
*/

/* Reads from a VGA Attribute Register */
/*
static uint8_t VGA_ReadAttribute(uint16_t index)
{
	PORT_READ8(0x3DA); 							//Set Flip/Flop to Index
	uint8_t adr = PORT_READ8(0x3C0); 			//Read Address Register
	PORT_WRITE8(0x3C0, 0x20 | index); 			//Index of Register to Write
	uint8_t val = PORT_READ8(0x3C1); 			//Read Data Register 
	PORT_WRITE8(0x3C0, val); 					//Write Back Data to Register (Prevents Corruption)
	PORT_WRITE8(0x3C0, adr); 					//Write Address Register Back
	PORT_READ8(0x3DA); 							//Set Flip/Flop to Index
	return val;
}
*/

/* Set Cursor Position */
static void VGA_SetCursor(int cX, int cY)
{
	uint16_t pos = cY * VGA_WIDTH + cX;
 
	VGA_CURSOR_X = cX;
	VGA_CURSOR_Y = cY;
	PORT_WRITE8(0x3D4, 0x0F);
	PORT_WRITE8(0x3D5, (uint8_t) (pos & 0xFF));
	PORT_WRITE8(0x3D4, 0x0E);
	PORT_WRITE8(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

/* INTERNAL: Foreground + Background Color Mix */
static inline uint8_t VGA_ColorMix(uint8_t fg, uint8_t bg) 
{
	return fg | bg << 4;
}

/* INTERNAL: Package Character/Color Entry into VGA Format */
static inline uint16_t VGA_CharEntry(unsigned char ch, uint8_t chColor) 
{
	return (uint16_t) ch | (uint16_t) chColor << 8;
}

/* Draw to Display (X,Y) */
static inline void VGA_Draw(char ch, size_t posX, size_t posY) 
{
	const size_t index = posY * VGA_WIDTH + posX;
	VGA_BUFFER[index] = (uint16_t) ch | (VGA_ColorMix(VGA_COLOR_FG,VGA_COLOR_BG) << 8);
}

/* Draw to Display (X,Y)(Custom Colors) */
static inline void VGA_DrawColored(char ch, size_t posX, size_t posY, uint8_t chFG, uint8_t chBG) 
{
	const size_t index = posY * VGA_WIDTH + posX;
	VGA_BUFFER[index] = (uint16_t) ch | (VGA_ColorMix(chFG,chBG) << 8);
}

/* Append String to Display (X,Y) */
static inline void VGA_DrawStr(char* str, size_t posX, size_t posY)
{
	for (size_t c = 0; c < StrLength(str); c++)
	{
		const size_t index = posY * VGA_WIDTH + posX;
		VGA_BUFFER[index] = (uint16_t) str[c] | (VGA_ColorMix(VGA_COLOR_FG,VGA_COLOR_BG) << 8);
		posX++;
		//Check Boundaries of VGA to Ensure Wrap
		if (posX > VGA_WIDTH) { posY++; posX = 0; } 
	}
}

/* Append String to Display (X,Y)(Custom Colors) */
static inline void VGA_DrawStrColored(char* str, size_t posX, size_t posY, uint8_t chFG, uint8_t chBG)
{
	for (size_t c = 0; c < StrLength(str); c++)
	{
		const size_t index = posY * VGA_WIDTH + posX;
		VGA_BUFFER[index] = (uint16_t) str[c] | (VGA_ColorMix(chFG,chBG) << 8);
		posX++;
		//Check Boundaries of VGA to Ensure Wrap
		if (posX > VGA_WIDTH) { posY++; posX = 0; } 
	}
}

/* Append Character to Display */
static inline void VGA_Write(char ch)
{
	const size_t index = VGA_CURSOR_Y * VGA_WIDTH + VGA_CURSOR_X;
	VGA_BUFFER[index] = (uint16_t) ch | (VGA_ColorMix(VGA_COLOR_FG,VGA_COLOR_BG) << 8);
	VGA_CURSOR_X++; 
	//Set Cursor
	VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
}

/* Append Character to Display */
static inline void VGA_WriteColored(char ch, uint8_t fg, uint8_t bg)
{
	const size_t index = VGA_CURSOR_Y * VGA_WIDTH + VGA_CURSOR_X;
	VGA_BUFFER[index] = (uint16_t) ch | (VGA_ColorMix(fg,bg) << 8);
	VGA_CURSOR_X++;
	//Set Cursor
	VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
}

/* Append String to Display */
static inline void VGA_WriteStr(char* str)
{
	for (size_t c = 0; c < StrLength(str); c++)
	{
		const size_t index = VGA_CURSOR_Y * VGA_WIDTH + VGA_CURSOR_X;
		VGA_BUFFER[index] = (uint16_t) str[c] | (VGA_ColorMix(VGA_COLOR_FG,VGA_COLOR_BG) << 8);
		VGA_CURSOR_X++;
	}
	//Set Cursor
	VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
}

/* Append String to Display */
static inline void VGA_WriteStrColored(char* str, uint8_t fg, uint8_t bg)
{
	for (size_t c = 0; c < StrLength(str); c++)
	{
		const size_t index = VGA_CURSOR_Y * VGA_WIDTH + VGA_CURSOR_X;
		VGA_BUFFER[index] = (uint16_t) str[c] | (VGA_ColorMix(fg,bg) << 8);
		VGA_CURSOR_X++;
	}
	//Set Cursor
	VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
}

/* VGA Backspace */
static inline void VGA_Back()
{
	//Attempt Running Backspace, Only if not in Top-Left Corner
	if (!(VGA_CURSOR_X > 1 && VGA_CURSOR_Y == 0))
	{
		//Remove Last Written Character
		VGA_Draw(' ', VGA_CURSOR_X - 1, VGA_CURSOR_Y);
		//Set Cursor
		VGA_CURSOR_X--;
		VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
	}
}

/* Clear Entire VGA Display */
static inline void VGA_Clear()
{
	//Run Through Each Row/Column Clearing Buffer
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
	{
		for (size_t x = 0; x < VGA_WIDTH; x++) 
		{
			VGA_Draw(' ', x, y);
		}
	}
}

/* Initialize Display */
static inline void VGA_Init(uint8_t fgColor, uint8_t bgColor)
{
	//Set VGA Vars to Value of Arguments
	VGA_CURSOR_X = 0;
	VGA_CURSOR_Y = 0;
	VGA_COLOR_FG = fgColor;
	VGA_COLOR_BG = bgColor;
	//Point VGA Buffer to VGA Memory Buffer Location
	VGA_BUFFER = (uint16_t*) 0xB8000;
	//Set Cursor
	VGA_SetCursor(VGA_CURSOR_X, VGA_CURSOR_Y);
	//Clear VGA
	VGA_Clear();
}

#endif
