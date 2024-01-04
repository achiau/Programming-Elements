CC = gcc
CFLAGS = -Wall -O3 -std=c99
OBJS = nn_base.o

test_nn: test_nn.c $(OBJS)
	$(CC) $(CFLAGS) -o test_nn test_nn.c $(OBJS)

nn_base.o: nn_base.c nn_base.h
	$(CC) $(CFLAGS) -c nn_base.c

clean:
	rm -f *.o test_nn
