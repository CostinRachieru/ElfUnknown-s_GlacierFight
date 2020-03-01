build:
	gcc -std=c99 -Wall -Wextra main.c -o snowfight -lm
clean:
	rm -rf snowfight
