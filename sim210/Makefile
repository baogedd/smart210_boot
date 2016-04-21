all: start.o mem_init.o main.o led.o
	arm-linux-ld -Tmyboot.lds -o myboot.elf $^
	arm-linux-objcopy -O binary myboot.elf 210boot.bin
	
%.o : %.S
	arm-linux-gcc -g -c $^
	
%.o : %.c 
	arm-linux-gcc -g -c $^
	
.PHONY: clean
clean:
	rm *.o *.elf *.bin


