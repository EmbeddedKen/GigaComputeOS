echo OFF
echo -----------------------------------------
echo OS Development ~ Kenneth Vorseth, 2018
echo Build System Options:
echo ~ B) Build Only
echo ~ R) Build and Run in QEMU
echo ~ T) Test Only, Run in QEMU
set /p val="Choice:"
echo -----------------------------------------

if /i "%val%"=="B" (
echo [Build Only Selected]
echo -Starting Build...
echo -Assembling Bootloader...
i686-elf-as src\boot.s -o obj/boot.o
echo -Compiling Freestanding C Kernel...
i686-elf-gcc -c src\kernel.c -o obj\kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wformat=0
echo -Linking...
i686-elf-gcc -T src\linker.ld -o iso\os.bin -ffreestanding -O2 -nostdlib obj\boot.o obj\kernel.o -lgcc
echo -Build Complete.
)

if /i "%val%"=="R" (
echo [Build and Run Selected]
echo -Starting Build...
echo -Assembling Bootloader...
i686-elf-as src\boot.s -o obj/boot.o
echo -Compiling Freestanding C Kernel Set1
i686-elf-gcc -c src\kernel.c -o obj\kernel.o -std=gnu99 -ffreestanding -O2 -Wformat=0
echo -Linking...
i686-elf-gcc -T src\linker.ld -o iso\os.bin -ffreestanding -O2 -nostdlib obj\boot.o obj\kernel.o -lgcc
echo -Build Complete.
echo -Running OS Kernel via QEMU...
qemu-system-i386 -kernel iso/os.bin -k en-us
)

if /i "%val%"=="T" (
echo -Running OS Kernel via QEMU...
qemu-system-i386 -kernel iso/os.bin -k en-us
)

echo -----------------------------------------
echo Task Complete.
pause



echo %val%