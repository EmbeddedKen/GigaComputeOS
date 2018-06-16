/* Keyboard Header File for OS Development */
/* Copyright 2018 Kenneth Vorseth          */
#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdbool.h> // <- FreeStanding C Headers
#include <stddef.h>	//
#include <stdint.h>	//
#include "io.h" 		//< Provides Hardware-Level I/O Port Operations Required
#include "ps2.h"		//< Provides PS/2 Controller Hardware-Level Access
#include "utility.h" //< Provides Utilities for FreeStanding Applications

/* Current Scancode Setting */
//Defined When Built: SCANCODE_SET1 or SCANCODE_SET3

/* ASCII + System Key Definitions */
/* Everything Above 0x1F is ASCII */
#define  ASCII_Boundary 		 0x1F //--[Boundary Limit]-------
#define	key_null			 0x00 //--[Special System Keys]--
#define	key_newline			 0x01
#define	key_enter			 0x02
#define	key_escape			 0x03
#define	key_caplock			 0x04
#define	key_shift			 0x05
#define	key_ctrl			 0x06
#define	key_alt				 0x07
#define	key_home			 0x08
#define	key_end				 0x09
#define	key_insert			 0x0A
#define	key_delete			 0x0B
#define	key_backspace		 0x0C
#define	key_tab				 0x0D
#define	key_fn1				 0x0E
#define	key_fn2				 0x0F
#define	key_fn3				 0x10
#define	key_fn4				 0x11
#define	key_fn5				 0x12
#define	key_fn6				 0x13
#define	key_fn7				 0x14
#define	key_fn8				 0x15
#define	key_fn9				 0x16
#define	key_fn10			 0x17
#define	key_fn11			 0x18 
#define	key_fn12			 0x19 //-------------------------

#define	key_space			 0x20 //--[Keys Based on ASCII]--
#define	key_exclamation		 0x21 
#define	key_quotation		 0x22 
#define	key_pound			 0x23
#define	key_percent			 0x25
#define	key_ampersand		 0x26
#define	key_apostrophe		 0x27
#define	key_l_par			 0x28
#define	key_r_par			 0x29
#define	key_star			 0x2A
#define	key_plus			 0x2B
#define	key_comma			 0x2C
#define	key_minus			 0x2D
#define	key_period			 0x2E
#define	key_f_slash			 0x2F
	
#define	key_zero 			 0x30
#define	key_one  			 0x31
#define	key_two  			 0x32
#define	key_three 			 0x33
#define	key_four			 0x34
#define	key_five			 0x35
#define	key_six				 0x36
#define	key_seven			 0x37
#define	key_eight			 0x38
#define	key_nine			 0x39
	
#define	key_colon			 0x3A
#define	key_semicolon		 0x3B
#define	key_less			 0x3C
#define	key_equal			 0x3D
#define	key_greater			 0x3E
#define	key_question		 0x3F
#define	key_at				 0x40
	
#define	key_A				 0x41
#define	key_B				 0x42
#define	key_C				 0x43
#define	key_D				 0x44
#define	key_E				 0x45
#define	key_F				 0x46
#define	key_G				 0x47
#define	key_H				 0x48
#define	key_I				 0x49
#define	key_J				 0x4A
#define	key_K				 0x4B
#define	key_L				 0x4C
#define	key_M				 0x4D
#define	key_N				 0x4E
#define	key_O				 0x4F
#define	key_P				 0x50
#define	key_Q				 0x51
#define	key_R				 0x52
#define	key_S				 0x53
#define	key_T				 0x54
#define	key_U				 0x55
#define	key_V				 0x56
#define	key_W				 0x57
#define	key_X				 0x58
#define	key_Y				 0x59
#define	key_Z				 0x5A
	
#define	key_l_bracket 		 0x5B
#define	key_b_slash			 0x5C
#define	key_r_bracket		 0x5D
#define	key_carrot			 0x5E
#define	key_underscore		 0x5F
#define	key_grave			 0x60
	
#define	key_a				 0x61
#define	key_b				 0x62
#define	key_c 				 0x63
#define	key_d				 0x64
#define	key_e				 0x65
#define	key_f 				 0x66
#define	key_g				 0x67
#define	key_h 				 0x68
#define	key_i 				 0x69
#define	key_j 				 0x6A
#define	key_k 				 0x6B
#define	key_l 				 0x6C
#define	key_m 				 0x6D
#define	key_n 				 0x6E
#define	key_o 				 0x6F
#define	key_p				 0x70
#define	key_q 				 0x71
#define	key_r 				 0x72
#define	key_s 				 0x73
#define	key_t				 0x74
#define	key_u				 0x75
#define	key_v				 0x76
#define	key_w				 0x77
#define	key_x				 0x78
#define	key_y				 0x79
#define	key_z				 0x7A
	
#define	key_l_brace			 0x7B
#define	key_pipe			 0x7C
#define	key_r_brace			 0x7D
	
#define	key_money_cent	 	 0xA2
#define	key_money_dollar 	 0x24
#define	key_money_euro	 	 0x80
#define	key_money_pound	 	 0xA3
#define	key_money_yen		 0xA5
	
#define	key_function		 0x83
#define	key_dot				 0x95
#define	key_tilde			 0x98
	
#define	key_trademark		 0x99
#define	key_copyright		 0xA9
#define	key_rtrademark		 0xAE

#define	key_section			 0xA7
#define	key_degree			 0xB0
#define	key_plusminus		 0xB1
#define	key_micro			 0xB5  //-------------------------

#define	key_up				 0xB6  //--[Arrow Keys]--
#define	key_down			 0xB7  
#define	key_right			 0xB8
#define 	key_left			 0xB9

/* Shift Key Modifier Table */
/* Input: ASCII Key			*/
const uint8_t shift_table[0xFF] =
{
	[key_a]=key_A,
	[key_b]=key_B,
	[key_c]=key_C,
	[key_d]=key_D,
	[key_e]=key_E,
	[key_f]=key_F,
	[key_g]=key_G,
	[key_h]=key_H,
	[key_i]=key_I,
	[key_j]=key_J,
	[key_k]=key_K,
	[key_l]=key_L,
	[key_m]=key_M,
	[key_n]=key_N,
	[key_o]=key_O,
	[key_p]=key_P,
	[key_q]=key_Q,
	[key_r]=key_R,
	[key_s]=key_S,
	[key_t]=key_T,
	[key_u]=key_U,
	[key_v]=key_V,
	[key_w]=key_W,
	[key_x]=key_X,
	[key_y]=key_Y,
	[key_z]=key_Z,
	[key_one]=key_exclamation,
	[key_two]=key_at,
	[key_three]=key_pound,
	[key_four]=key_money_dollar,
	[key_five]=key_percent,
	[key_six]=key_carrot,
	[key_seven]=key_ampersand,
	[key_eight]=key_star,
	[key_nine]=key_l_par,
	[key_zero]=key_r_par,
	[key_minus]=key_underscore,
	[key_equal]=key_plus,
	[key_l_bracket]=key_l_brace,
	[key_r_bracket]=key_r_brace,
	[key_semicolon]=key_colon,
	[key_apostrophe]=key_quotation,
	[key_comma]=key_less,
	[key_period]=key_greater,
	[key_f_slash]=key_question,
	[key_b_slash]=key_pipe
};



/* Represents Scancodes in Released Form */
/* Subtract 0x80 to Obtain Pressed Form  */
/* Zero Represents Unsupported           */
#define Pressed_Offset		0x80
const uint8_t scancodes[0xFF] =
{
	[0x00]=0x00,
	[0x01]=key_escape,
	[0x02]=key_one,
	[0x03]=key_two,
	[0x04]=key_three,
	[0x05]=key_four,
	[0x06]=key_five,
	[0x07]=key_six,
	[0x08]=key_seven,
	[0x09]=key_eight,
	[0x0A]=key_nine,
	[0x0B]=key_zero,
	[0x0C]=key_minus,
	[0x0D]=key_equal,
	[0x0E]=key_backspace,
	[0x0F]=key_tab,
	[0x10]=key_q,
	[0x11]=key_w,
	[0x12]=key_e,
	[0x13]=key_r,
	[0x14]=key_t,
	[0x15]=key_y,
	[0x16]=key_u,
	[0x17]=key_i,
	[0x18]=key_o,
	[0x19]=key_p,
	[0x1A]=key_l_brace,
	[0x1B]=key_r_brace,
	[0x1C]=key_enter,
	[0x1D]=key_ctrl,
	[0x1E]=key_a,
	[0x1F]=key_s,
	[0x20]=key_d,
	[0x21]=key_f,
	[0x22]=key_g,
	[0x23]=key_h,
	[0x24]=key_j,
	[0x25]=key_k,
	[0x26]=key_l,
	[0x27]=key_semicolon,
	[0x28]=key_apostrophe,
	[0x29]=key_grave,
	[0x2A]=key_shift,
	[0x2B]=key_b_slash,
	[0x2C]=key_z,
	[0x2D]=key_x,
	[0x2E]=key_c,
	[0x2F]=key_v,
	[0x30]=key_b,
	[0x31]=key_n,
	[0x32]=key_m,
	[0x33]=key_comma,
	[0x34]=key_period,
	[0x35]=key_f_slash,
	[0x36]=key_shift,
	[0x38]=key_alt,
	[0x39]=key_space,
	[0x3A]=key_caplock,
	[0x3B]=key_fn1,
	[0x3C]=key_fn2,
	[0x3D]=key_fn3,
	[0x3E]=key_fn4,
	[0x3F]=key_fn5,
	[0x40]=key_fn6,
	[0x41]=key_fn7,
	[0x42]=key_fn8,
	[0x43]=key_fn9,
	[0x44]=key_fn10,
	[0x57]=key_fn11,
	[0x58]=key_fn12
};

/* Table Holds Flags For Each Key Pressed */
/* xxxx xxLC L = Last, C = Current 		  */
uint8_t key_states[0xFF] = {0x00};
#define KEY_CURRENT 		0
#define KEY_LAST	   		1

/* Keyboard Scan Codes Functions       */                  
/* Poll Port 0x60 (PS/2 #1 Data Port)  */
/* Various Key Get Functions           */
/* 			Relies on Keyboard_Update()*/
/*				   Calling Every Cycle */
static inline uint8_t Keyboard_GetKey(uint8_t key)
{
	//Check if Key Currently Pressed
	return IsBitSet(key_states[key], KEY_CURRENT);
}
static inline uint8_t Keyboard_GetKeyRelease(uint8_t key)
{
	//Event: On Key Release
	return IsBitClr(key_states[key], KEY_CURRENT) && IsBitSet(key_states[key], KEY_LAST);
}
static inline uint8_t Keyboard_GetKeyPress(uint8_t key)
{
	//Event: On Key Press
	return IsBitSet(key_states[key], KEY_CURRENT) && IsBitClr(key_states[key], KEY_LAST);
}

/* To Be Called Once Every Cycle */
/* Updates Internal Key States   */
/* Returns Printable Characters  */
static char Keyboard_Update()
{
	//Iterate Through all Keys in Status Table
	//Write Current Bit State to Last Bit State
	for (size_t k = 0; k < 0xFE; k++)
	{
		if (IsBitSet(key_states[k], KEY_CURRENT)) { SetBit(key_states[k], KEY_LAST); }
		else { ClrBit(key_states[k], KEY_LAST); }
	}
	
	//Check if PS/2 Status Register Indicates
	//Output Buffer is Full
	if (PORT_READ8(PS2_REG_STAT) & 0x01)
	{
		//If so, Read PS/2 Data Port
		uint8_t data = PORT_READ8(PS2_REG_DATA);
		//Set New Current State Based on Port Read
		//Determine Whether Key Press or Release
		if (data < 0x80) //Key Press
		{
			//Flag as Currently Pressed
			data = scancodes[data];
			SetBit(key_states[data], KEY_CURRENT);
			//Return Any Printable Characters
			if (IsBitClr(key_states[data], KEY_LAST))
			{ return (data > ASCII_Boundary) ? data : 0x00; }
		}
		else //Key Release
		{
			//Decrease Data by Offset (Pressed# = Release# - 0x80)
			data -= 0x80;
			//Flag as Not Pressed
			data = scancodes[data];
			ClrBit(key_states[data], KEY_CURRENT);
		}
	}
	return 0x00; //No Toggled Printable Characters to Return this Call
}

#endif 