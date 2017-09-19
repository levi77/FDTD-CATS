sumarrays: sumarrayrows.c sumarraycols.c
	gcc -g sumarrayrows.c -o sumarrayrows
	gcc -g sumarraycols.c -o sumarraycols

perf-rows: sumarrayrows.c
	perf stat -d ./sumarrayrows
		
perf-cols: sumarraycols.c
	perf stat -d ./sumarraycols

valgrind-rows: sumarrayrows.c
	$(CC) $(CFLAGS) sumarrayrows.c  -lm -o sumarrayrows
	valgrind --tool=cachegrind --cachegrind-out-file=outrows ./sumarrayrows
	
valgrind-cols: sumarraycols.c
	$(CC) $(CFLAGS) sumarraycols.c  -lm -o sumarraycols
	valgrind --tool=cachegrind --cachegrind-out-file=outcols ./sumarraycols

clean:
	rm -f sumarraycols *.o *~
	rm -f sumarrayrows *.o *~



