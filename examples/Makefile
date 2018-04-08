TARGET = example
CFLAGS = `pkg-config --cflags glfw3` -Wall -Wextra -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL
INCLUDE = -I../include

$(TARGET):
	g++ $(CFLAGS) $(INCLUDE) -c -o $@.o $@.cpp
	g++ $@.o ../lib/libariamis.a -o $@ $(LIBS)

clean:
	rm -f $(TARGET) $(TARGET).o
