main: main.o process_control.o process.o
	gcc -std=c99 main.o process_control.o process.o -o main
main.o: main.c Makefile
	gcc -std=c99 main.c -c
scheduler.o: process_control.c process_control.h Makefile
	gcc -std=c99 process_control.c -c
process.o: process.c process.h Makefile
	gcc -std=c99 process.c -c
clean:
	rm -rf *o
run:
	sudo ./main  
