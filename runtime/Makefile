CC = gcc
AR = ar
CFLAGS = -Wall -O2

OBJS = rt_arena.o rt_atomic.o rt_string.o rt_io.o rt_async.o rt_net.o rt_crypto.o rt_thread.o rt_process.o nyx_mlir_backend.o vm.o security.o

libnyxrt.a: $(OBJS)
	$(AR) rcs libnyxrt.a $(OBJS)

rt_arena.o: rt_arena.c rt_arena.h
	$(CC) $(CFLAGS) -c rt_arena.c

rt_string.o: rt_string.c rt_string.h
	$(CC) $(CFLAGS) -c rt_string.c

rt_io.o: rt_io.c rt_io.h
	$(CC) $(CFLAGS) -c rt_io.c

rt_async.o: rt_async.c rt_async.h
	$(CC) $(CFLAGS) -c rt_async.c

rt_net.o: rt_net.c rt_net.h
	$(CC) $(CFLAGS) -c rt_net.c

rt_crypto.o: rt_crypto.c rt_crypto.h
	$(CC) $(CFLAGS) -c rt_crypto.c

rt_thread.o: rt_thread.c rt_thread.h
	$(CC) $(CFLAGS) -c rt_thread.c

rt_process.o: rt_process.c
	$(CC) $(CFLAGS) -c rt_process.c

nyx_mlir_backend.o: nyx_mlir_backend.c nyx_mlir_backend.h
	$(CC) $(CFLAGS) -c nyx_mlir_backend.c

vm.o: vm.c vm.h
	$(CC) $(CFLAGS) -c vm.c

security.o: security.c vm.h
	$(CC) $(CFLAGS) -c security.c

clean:
	rm -f *.o libnyxrt.a

rt_atomic.o: rt_atomic.c rt_atomic.h
	$(CC) $(CFLAGS) -std=c11 -c rt_atomic.c
