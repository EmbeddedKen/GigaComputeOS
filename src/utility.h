/* Utilities for Freestanding Applications */
/* Copyright 2018 Kenneth Vorseth          */
#ifndef UTILITY_H
#define UTILITY_H

/* Returns Length of NULL-Terminated C-Strings */
inline size_t StrLength(char* str) 
{
	size_t len;
	for (len = 0; str[len] != '\0'; len++);
	return len;
}
/* Copies C-Strings of Same Length */
inline void StrCopy(char* strSource, char* strDestination)
{
	//Ensures Strings are Same Length
	uint8_t len = StrLength(strSource);
	if (len != StrLength(strDestination)) { return; }
	//Perform Copy
	for (int c = 0; c < len; c++)
	{
		strDestination[c] = strSource[c];
	}
}

/* Converts Byte to Hex String */
const char hex_codes[0x10] = 
{
	[0x00]='0',[0x01]='1',[0x02]='2',[0x03]='3',
	[0x04]='4',[0x05]='5',[0x06]='6',[0x07]='7',
	[0x08]='8',[0x09]='9',[0x0A]='A',[0x0B]='B',
	[0x0C]='C',[0x0D]='D',[0x0E]='E',[0x0F]='F'
};
char hex[0x05];
char* ToHex(uint8_t val)
{
	hex[0x00] = '0'; hex[0x01] = 'x';
	hex[0x02] = hex_codes[(val & 0xF0) >> 4];
	hex[0x03] = hex_codes[(val & 0x0F)]; 
	hex[0x04] = '\0';
	return hex; 
}
char* ToHexShort(uint8_t val)
{
	hex[0x00] = hex_codes[(val & 0xF0) >> 4];
	hex[0x01] = hex_codes[(val & 0x0F)]; 
	hex[0x02] = '\0';
	return hex; 
}
char* ToHexShort16(uint8_t val)
{
	hex[0x00] = hex_codes[(val & 0xF000) >> 12];
	hex[0x01] = hex_codes[(val & 0x0F00) >> 8]; 
	hex[0x02] = hex_codes[(val & 0x00F0) >> 4];
	hex[0x03] = hex_codes[(val & 0x000F)]; 
	hex[0x04] = '\0';
	return hex; 
}

/* Returns Whether Specified Bit is Set */
#define IsBitSet(val,bit)		(val & (0x01 << bit))

/* Returns Whether Specified Bit is Set */
#define IsBitClr(val,bit)		(!(val & (0x01 << bit)))

/* Set Bit in Mask */
#define SetBit(val,bit)		(val |= (0x01 << bit))

/* Clear Bit in Mask */
#define ClrBit(val,bit)		(val &= ~(0x01 << bit))

#endif
