CC=g++
TARGET=cross
LDFLAGS=-pthread
CFLAGS=-O2
CXXFLAGS=-O2
OBJS= main.o cross.o car.o thread.o direction.o deadlock.o
default: $(TARGET)
$(TARGET): $(OBJS)
main.o: cross.h
cross.o: cross.h car.h deadlock.h direction.h mutex.h
car.o: car.h deadlock.h direction.h mutex.h sem.h thread.h
thread.o: thread.h
direction.o: direction.h
deadlock.o: deadlock.h mutex.h sem.h thread.h deadlock.h car.h cross.h direction.h
clean:
	rm -f $(TARGET) $(OBJS)
