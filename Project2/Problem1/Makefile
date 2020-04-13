all:
	gcc -fsanitize=address -w -g -o thread threads.c -lm -pthread

one:
	gcc -fsanitize=address -w -g -o thread one_thread.c -lm -pthread
variation:
	gcc -fsanitize=address -w -g -o thread threads1.c -lm -pthread

clean:
	rm -rf thread
