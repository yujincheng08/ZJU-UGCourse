CC=g++
CXXFLAGS=-O2 -DGLEW_STATIC
LDFLAGS= -static -lglew32 -lopengl32 -lglu32 -lglfw3 -lgdi32
TARGET=solar
OBJS=main.o camera.o mesh.o scene.o solar.o sphere.o
default:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)
clean:
	rm -rf $(OBJS) $(TARGET)
