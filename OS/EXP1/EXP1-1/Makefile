CC=g++
TARGET=cross
LDFLAGS=-pthread
CFLAGS=-O2
CXXFLAGS=-O2
OBJS= main.o cross.o car.o thread.o direction.o deadlock.o
default: $(TARGET)
$(TARGET): $(OBJS)
clean:
	rm -f $(TARGET) $(OBJS)
