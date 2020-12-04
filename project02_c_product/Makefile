build:
	gcc -Wall -Werror -Wextra -std=c99 ./src/*.c -lm -lSDL2 -o raycast

debug:
	gcc -Wall -Werror -Wextra -g -fsanitize=address -std=c99 ./src/*.c -lm -lSDL2 -o raycast

run:
	./raycast

clean:
	rm -f raycast