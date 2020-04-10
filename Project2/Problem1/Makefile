all:
	gcc -lm -pthread -fsanitize=address -w -g -o thread threads.c

variation:
	gcc -lm -pthread -fsanitize=address -w -g -o thread threads1.c

clean:
	rm -rf thread
