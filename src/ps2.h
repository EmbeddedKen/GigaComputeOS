/* PS/2 Controller Driver for OS Development */
/* Copyright 2018 Kenneth Vorseth            */
#ifndef PS2_H
#define PS2_H
#include <stdbool.h> // <- FreeStanding C Headers
#include <stddef.h>	//
#include <stdint.h>	//
#include "io.h" 		//< Provides Hardware-Level I/O Port Operations Required
#include "utility.h" //< Provides Utilities for FreeStanding Applications

/* Keyboard Initialize | POST			*/
/* Setup Hardware and Check Keyboard	*/
/* Keyboard Connection      			*/
#define PS2_ACK				0xFA //< Special Bytes
#define PS2_RESEND			0xFE //
#define PS2_SUCCESS			0xAA // 
#define PS2_FAIL1			0xFC //
#define PS2_FAIL2			0xFD //
#define PS2_REG_CMD			0x64 //< Registers|Ports
#define PS2_REG_STAT		0x64 //
#define PS2_REG_DATA		0x60 //

static inline void PS2_Write(uint8_t reg, uint8_t data)
{
	//Ensure Input Buffer is Empty Before Writing
	while (IsBitSet(PORT_READ8(PS2_REG_STAT), 0x01));
	//Write to Input Buffer
	PORT_WRITE8(reg, data);
}
static inline uint8_t PS2_ReadResponse()
{
	//Ensure Output Buffer is Filled Before Reading
	while (IsBitClr(PORT_READ8(PS2_REG_STAT), 0x00));
	//Read From Output Buffer
	return PORT_READ8(PS2_REG_DATA);
}

static uint8_t PS2_POST(void)
{
	//PS2 POST Return Value
	//Bit0: Has Port1  (0=No Port  1=Has Port)
	//Bit1: Port1 Type (0=Keyboard 1=Mouse)
	//Bit2: Has Port2  (0=No Port  1=Has Port)
	//Bit3: Port2 Type (0=Keyboard 1=Mouse)
	//Bit4: PS2 Error  (0=No Error) (1=Error)
	uint8_t ret = 0x00;
	
	//Send Commands to Disable PS/2 Controller Ports
	PS2_Write(PS2_REG_CMD, 0xAD);
	PS2_Write(PS2_REG_CMD, 0xA7);
	//Perform Controller Self-Test
	PS2_Write(PS2_REG_CMD, 0xAA);
	if (PS2_ReadResponse() == 0x55)
	{
		//Flush PS/2 Controller Data Buffer (Discarded Read)
		PORT_READ8(PS2_REG_DATA);
		//Read PS/2 Controller Configuration
		PS2_Write(PS2_REG_CMD, 0x20);				//Request to Recieve Configuration
		uint8_t cfg = PS2_ReadResponse();				//Receive Current Configuration
		if (IsBitSet(cfg, 5)) { SetBit(ret, 2); } 		//Check if Dual-Channel Controller
		//Write PS/2 Controller Configuration
		cfg = 0x04; 								//Disable IRQs + Disable Translation
		PS2_Write(PS2_REG_CMD, 0x60);		   		//Request to Configure Controller
		PS2_Write(PS2_REG_DATA, cfg);		   			//Actual Write of Config Data
		//Test First PS/2 Port
		PS2_Write(PS2_REG_CMD, 0xAB);
		if (PS2_ReadResponse() == 0x00) { SetBit(ret,0); }
		//Test Second PS/2 Port
		PS2_Write(PS2_REG_CMD, 0xA9);
		if (PS2_ReadResponse() == 0x00) { SetBit(ret,2); }
		
		//Re-Enable Ports and Identify
		if (IsBitSet(ret, 0x00)) 
		{ 
			//Enable First PS/2 Port
			PS2_Write(PS2_REG_CMD, 0xAE); 
			//Identify First PS/2 Port
			PS2_Write(PS2_REG_DATA, 0xF5);		//To Device: Disable Scanning
			PS2_ReadResponse(); 					//ACK
			PS2_Write(PS2_REG_DATA, 0xF2);		//To Device: Identify
			PS2_ReadResponse(); 					//ACK
			//Mark PS/2 Port as Mouse (Otherwise Keyboard)
			uint8_t resp = PS2_ReadResponse();
			if (resp != 0xAB && resp != 0x83)
			{ SetBit(ret, 0x01); }
		}
		if (IsBitSet(ret, 0x02)) 
		{ 
			//Enable Second PS/2 Port
			PS2_Write(PS2_REG_CMD, 0xA8); 
			//Identify Second PS/2 Port
			PS2_Write(PS2_REG_CMD, 0xD4);		//To Controller: Send to Second Device
			PS2_Write(PS2_REG_DATA, 0xF5);		//To Device: Disable Scanning
			PS2_ReadResponse(); 					//ACK
			PS2_Write(PS2_REG_CMD, 0xD4);		//To Controller: Send to Second Device
			PS2_Write(PS2_REG_DATA, 0xF2);		//To Device: Identify
			PS2_ReadResponse(); 					//ACK
			PS2_ReadResponse(); 					//ACK
			//Mark PS/2 Port as Mouse (Otherwise Keyboard)
			if (PS2_ReadResponse() == 0x00)
			{ SetBit(ret, 0x03); }
		}
		
		//First PS/2 Port as Keyboard | Specific Settings
		if (IsBitSet(ret, 0x00) && IsBitClr(ret, 0x01))
		{
			//Set Keyboard Scan Code Set to #1
			PS2_Write(PS2_REG_DATA, 0xF0);		//To Device: Change Scancode Set
			PS2_ReadResponse();					//ACK
			PS2_Write(PS2_REG_DATA, 0x01);		//To Device: Scancode Set #1
		}
		
		//If Both Ports Are Non-Functional, Flag Critical Error
		if (IsBitClr(ret, 0x00) && IsBitClr(ret, 0x02)) { SetBit(ret, 0x04); }
	}
	else //PS/2 Controller Failed Self-Test
	{
		//Flag as Critical Controller Error
		SetBit(ret, 0x04);
	}
	
	//Return Status/Error Code
	return ret;
}

#endif
