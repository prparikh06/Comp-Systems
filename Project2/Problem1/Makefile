all:
	gcc -lm -pthread -fsanitize=address -w -g -o thread threads.c

one:
	gcc -lm -pthread -fsanitize=address -w -g -o thread one_thread.c
variation:
	gcc -lm -pthread -fsanitize=address -w -g -o thread threads1.c

clean:
	rm -rf thread
