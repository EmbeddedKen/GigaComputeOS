/* Interrupt Handler for x86 Protected Mode */
/* Copyright 2018 Kenneth Vorseth			*/
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/* Defined Bits in Access Byte of GDT */
#define SEG_PR 		7		//< 1Bit : Present 			(Typically 1)
#define SEG_PRIVL	5		//< 2Bit : Priviledge Level (0=Kernel,...,3=Application)
#define SEG_ONE		4		//< 1Bit : Always Set to 1
#define SEG_EXEC		3		//< 1Bit : Executable 		(1=Executable, 0=Data Section)
#define SEG_DC		2		//< 1Bit : Dir/Conform 		(1=Equal or Lower Priv, 0=Equal Priv)
#define SEG_RW		1		//< 1Bit : Read/Write 		(CS: Read Allowed?, DS: Write Allowed?)
#define SEG_ACC		0		//< 1Bit : Segment Accessed (Set to 0, CPU Sets to 1 when Accessed)
/* Defined Bits in LimitH Offset (FLAGS) */
#define SEG_GR		7		//< 1Bit : Granularity 		(Limit Measured in: 0=1B, 1=4KiB Blocks)

/* Global Descriptor Table Entry */
/* Maximum Addressable Range = Base + Limit Offset */
struct GDTDesc
{
	uint16_t seg_limitL;		//< Limit Offset (0:15)
	uint16_t seg_baseL;		//< Base Address (0:15)
	uint8_t	seg_baseM;		//< Base Address (16:23)
	uint8_t	seg_access;		//< Bit-Field : Access Byte
	uint8_t	seg_limitH;		//< Limit Offset (16:19) [Lower] | [Upper] Flags 
	uint8_t	seg_baseH;		//< Base Address (24:31)
} __attribute__((packed));

struct GDTDesc tableGDT[3];
static inline void Write_GDT(struct GDTDesc* tablePtr)
{
	asm volatile ( "LGDT %0;
					JMP 0x08:%EIP;" : :"a"(tablePtr));
	asm volatile ( "mov ax, 0x10;
					mov ds, ax; 
					mov es, ax;
					mov fs, ax; 
					mov gs, ax; 
					mov ss, ax" );
}

static void Setup_GDT()
{
	//Create Null Descriptor Entry
	struct GDTDesc entryNull;
	entryNull.seg_limitL = 0x0000;
	entryNull.seg_baseL  = 0x0000;
	entryNull.seg_access = 0x00;
	entryNull.seg_limitH = 0x00;
	entryNull.seg_baseH  = 0x00;
	tableGDT[0] = entryNull;
	//Create Code Segment Entry [SELECTOR: 0x08]
	struct GDTDesc entryCS;
	entryCS.seg_limitL   = 0xFFFF;
	entryCS.seg_baseL    = 0x0000;
	entryCS.seg_access	 = 0x9A;
	entryCS.seg_access	 = (1 << SEG_RW) | (1 << SEG_EXEC) | (1 << SEG_ONE) | (1 << SEG_PR);
	entryCS.seg_limitH	 = 0xCF;
	entryCS.seg_baseH	 = 0x00;
	tableGDT[1] = entryCS;
	//Create Data Segment Entry [SELECTOR: 0x10]
	struct GDTDesc entryDS;
	entryDS.seg_limitL	 = 0xFFFF;
	entryDS.seg_baseL    = 0x0000;
	entryDS.seg_access   = (1 << SEG_RW) | (1 << SEG_ONE) | (1 << SEG_PR);
	entryDS.seg_limitH   = 0xCF;
	entryDS.seg_baseH	 = 0x00;
	tableGDT[2] = entryDS;
	//Write Global Descriptor Table
	Write_GDT(&tableGDT);
}

/* Defined Interrupt Types */
#define GATE_INT		0xE		//< Upon Interrupt Entry, Interrupts are Disabled
#define GATE_TRAP	0xF		//< Upon Interrupt Entry, Interrupts are Enabled (Manual Control)
/* Interrupt Descriptor Struct */
struct IDTDesc
{
	uint16_t offset_1;
	uint16_t selector;
	uint8_t  zero;
	uint8_t  type;
	uint16_t offset_2;
} __attribute__((packed));



#endif
