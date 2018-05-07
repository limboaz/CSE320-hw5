all:
	gcc	-g	main.c	-lpthread	-o	main
	gcc -g mem.c	-o	mem

clean:
	rm	fifo_mem	fifo_main	main	mem
