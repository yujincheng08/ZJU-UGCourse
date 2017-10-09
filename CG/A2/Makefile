CC=g++
CXXFLAGS=-O2
LDFLAGS=-lglew32 -lfreeglut -lopengl32 -lglu32
TARGET=flag
OBJS=flag.o
default:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
clean:
	rm -rf $(OBJS) $(TARGET)
