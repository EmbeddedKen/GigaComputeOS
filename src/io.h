/* Hardware-Level x86 I/O              */
/* From wiki.osdev.org/Inline_Assembly */
#ifndef IO_H
#define IO_H
#include <stdbool.h>	// <- FreeStanding C Headers
#include <stddef.h>	//
#include <stdint.h>	//

/* x86 DISABLE INTERRUPTS */
inline void INT_DISABLE()
{
	asm volatile ( "cli" );
}
inline void INT_ENABLE()
{
	asm volatile ( "sei" );
}

/* x86 PORT WRITES {BWL- Byte8/Word16/Long32} */
inline void PORT_WRITE8(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
inline void PORT_WRITE16(uint16_t port, uint16_t val)
{
	asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}
inline void PORT_WRITE32(uint16_t port, uint32_t val)
{
	asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

/* x86 PORT READS {BWL- Byte8/Word16/Long32} */
inline uint8_t PORT_READ8(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}
inline uint16_t PORT_READ16(uint16_t port)
{
    uint16_t ret;
    asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}
inline uint32_t PORT_READ32(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}
#endif
