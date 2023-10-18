all: app_main

app_main: app_main.o func.o
	gcc obj/app_main.o obj/func.o -o bin/app_main.out	

func.o: src/func.c include/func.h
	gcc -I include -Wall -Werror -c src/func.c -o obj/func.o

app_main.o: apps/app_main.c include/func.h
	gcc -I include -Wall -Werror -c apps/app_main.c -o obj/app_main.o

run:
	./bin/app_main.out	

clean:
	del bin\app_main.exe obj\*.o 	