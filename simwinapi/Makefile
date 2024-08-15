main : main.o console.o window.o keyboard.o desktop.o
	gcc *.o -o main

%.o : %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm *.o *.exe
