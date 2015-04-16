
CC = arm-none-eabi-

INC = 	-Ikernel/include \
		-Ikernel/portable/GCC/ARM_CM3 \
		-Idevice \
		-Iconfig \
		-Idrivers \
		-Isyscalls \
		-Iminilib/include \
		-Itest

VPATH = kernel: \
		kernel/portable/GCC/ARM_CM3: \
		kernel/portable/MemMang: \
		app: \
		device: \
		drivers: \
		syscalls: \
		minilib/ctype: \
		minilib/stdio: \
		minilib/stdlib: \
		minilib/string: \
		test

LDSCRIPT = linker.ld

CFLAGS = 	-g -c -Wall -Os -mthumb \
 			-mcpu=cortex-m3 $(INC) -std=gnu99 
LFLAGS = 	-g -Wall -Os -mthumb -mcpu=cortex-m3 -nostartfiles \
			-nodefaultlibs \
			-T$(LDSCRIPT) 

OUTPUT = app.elf

OBJS = 	list.o \
		queue.o \
		tasks.o \
		timers.o \
		port.o \
		heap_2.o \
		lpc17xx.o \
		system_LPC17xx.o \
		main.o \
		uart.o \
		syscalls.o \
		gsm.o \
		test.o \
		ctype_.o \
		isalnum.o \
		isalpha.o \
		isascii.o \
		isblank.o \
		iscntrl.o \
		isdigit.o \
		islower.o \
		isprint.o \
		ispunct.o \
		isspace.o \
		isupper.o \
		isxdigit.o \
		toascii.o \
		tolower.o \
		toupper.o \
		fclose.o \
		fflush.o \
		fgetc.o \
		fgetlen.o \
		fgets.o \
		fopen.o \
		fputc.o \
		fputs.o \
		getc.o \
		getchar.o \
		gets.o \
		printf.o \
		putc.o \
		putchar.o \
		puts.o \
		rget.o \
		scanf.o \
		setbuffer.o \
		wbuf.o \
		abs.o \
		assert.o \
		atoi.o \
		atol.o \
		calloc.o \
		div.o \
		exit.o \
		ldiv.o \
		malloc.o \
		rand.o \
		strtol.o \
		bcmp.o \
		bcopy.o \
		bzero.o \
		index.o \
		memccpy.o \
		memchr.o \
		memcmp.o \
		memcpy.o \
		memmove.o \
		mempcpy.o \
		memset.o \
		rindex.o \
		strcat.o \
		strchr.o \
		strcmp.o \
		strcoll.o \
		strcpy.o \
		strcspn.o \
		strlcat.o \
		strlcpy.o \
		strlen.o \
		strlwr.o \
		strncat.o \
		strncmp.o \
		strncpy.o \
		strnlen.o \
		strrchr.o \
		strspn.o \
		strstr.o \
		strupr.o

all: $(OBJS)
	$(CC)gcc $(LFLAGS) $(OBJS) -o $(OUTPUT)
	$(CC)size $(OUTPUT)

clean: $(OBJS)
	-rm $(OBJS)

list.o: list.c
	$(CC)gcc $(CFLAGS) $^ -o $@

queue.o: queue.c
	$(CC)gcc $(CFLAGS) $^ -o $@

tasks.o: tasks.c
	$(CC)gcc $(CFLAGS) $^ -o $@

timers.o: timers.c
	$(CC)gcc $(CFLAGS) $^ -o $@

port.o: port.c
	$(CC)gcc $(CFLAGS) $^ -o $@

heap_1.o: heap_1.c
	$(CC)gcc $(CFLAGS) $^ -o $@

heap_2.o: heap_2.c
	$(CC)gcc $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC)gcc $(CFLAGS) $^ -o $@

uart.o: uart.c
	$(CC)gcc $(CFLAGS) $^ -o $@

lpc17xx.o: lpc17xx.S
	$(CC)gcc $(CFLAGS) $^ -o $@

system_LPC17xx.o: system_LPC17xx.c
	$(CC)gcc $(CFLAGS) $^ -o $@

syscalls.o: syscalls.c
	$(CC)gcc $(CFLAGS) $^ -o $@

delay.o: delay.c
	$(CC)gcc $(CFLAGS) $^ -o $@

gsm.o: gsm.c
	$(CC)gcc $(CFLAGS) $^ -o $@

test.o: test.c
	$(CC)gcc $(CFLAGS) $^ -o $@

printf_stdarg.o: printf_stdarg.c
	$(CC)gcc $(CFLAGS) $^ -o $@

# Standard Library
ctype_.o: ctype_.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isalnum.o: isalnum.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isalpha.o: isalpha.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isascii.o: isascii.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isblank.o: isblank.c
	$(CC)gcc $(CFLAGS) $^ -o $@

iscntrl.o: iscntrl.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isdigit.o: isdigit.c
	$(CC)gcc $(CFLAGS) $^ -o $@

islower.o: islower.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isprint.o: isprint.c
	$(CC)gcc $(CFLAGS) $^ -o $@

ispunct.o: ispunct.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isspace.o: isspace.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isupper.o: isupper.c
	$(CC)gcc $(CFLAGS) $^ -o $@

isxdigit.o: isxdigit.c
	$(CC)gcc $(CFLAGS) $^ -o $@

toascii.o: toascii.c
	$(CC)gcc $(CFLAGS) $^ -o $@

tolower.o: tolower.c
	$(CC)gcc $(CFLAGS) $^ -o $@

toupper.o: toupper.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fclose.o: fclose.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fflush.o: fflush.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fgetc.o: fgetc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fgetlen.o: fgetlen.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fgets.o: fgets.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fopen.o: fopen.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fputc.o: fputc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

fputs.o: fputs.c
	$(CC)gcc $(CFLAGS) $^ -o $@

getc.o: getc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

getchar.o: getchar.c
	$(CC)gcc $(CFLAGS) $^ -o $@

gets.o: gets.c
	$(CC)gcc $(CFLAGS) $^ -o $@

printf.o: printf.c
	$(CC)gcc $(CFLAGS) $^ -o $@

putc.o: putc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

putchar.o: putchar.c
	$(CC)gcc $(CFLAGS) $^ -o $@

puts.o: puts.c
	$(CC)gcc $(CFLAGS) $^ -o $@

rget.o: rget.c
	$(CC)gcc $(CFLAGS) $^ -o $@

scanf.o: scanf.c
	$(CC)gcc $(CFLAGS) $^ -o $@

setbuffer.o: setbuffer.c
	$(CC)gcc $(CFLAGS) $^ -o $@

wbuf.o: wbuf.c
	$(CC)gcc $(CFLAGS) $^ -o $@

abs.o: abs.c
	$(CC)gcc $(CFLAGS) $^ -o $@

assert.o: assert.c
	$(CC)gcc $(CFLAGS) $^ -o $@

atoi.o: atoi.c
	$(CC)gcc $(CFLAGS) $^ -o $@

atol.o: atol.c
	$(CC)gcc $(CFLAGS) $^ -o $@

calloc.o: calloc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

div.o: div.c
	$(CC)gcc $(CFLAGS) $^ -o $@

exit.o: exit.c
	$(CC)gcc $(CFLAGS) $^ -o $@

ldiv.o: ldiv.c
	$(CC)gcc $(CFLAGS) $^ -o $@

malloc.o: malloc.c
	$(CC)gcc $(CFLAGS) $^ -o $@

rand.o: rand.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strtol.o: strtol.c
	$(CC)gcc $(CFLAGS) $^ -o $@

bcmp.o: bcmp.c
	$(CC)gcc $(CFLAGS) $^ -o $@

bcopy.o: bcopy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

bzero.o: bzero.c
	$(CC)gcc $(CFLAGS) $^ -o $@

index.o: index.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memccpy.o: memccpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memchr.o: memchr.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memcmp.o: memcmp.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memcpy.o: memcpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memmove.o: memmove.c
	$(CC)gcc $(CFLAGS) $^ -o $@

mempcpy.o: mempcpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

memset.o: memset.c
	$(CC)gcc $(CFLAGS) $^ -o $@

rindex.o: rindex.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strcat.o: strcat.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strchr.o: strchr.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strcmp.o: strcmp.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strcoll.o: strcoll.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strcpy.o: strcpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strcspn.o: strcspn.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strlcat.o: strlcat.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strlcpy.o: strlcpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strlen.o: strlen.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strlwr.o: strlwr.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strncat.o: strncat.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strncmp.o: strncmp.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strncpy.o: strncpy.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strnlen.o: strnlen.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strrchr.o: strrchr.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strspn.o: strspn.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strstr.o: strstr.c
	$(CC)gcc $(CFLAGS) $^ -o $@

strupr.o: strupr.c
	$(CC)gcc $(CFLAGS) $^ -o $@
