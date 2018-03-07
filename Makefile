SRCDIR = src
BUILDDIR = build
TARGET = bin/graphic
SRCEXT := cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))
CFLAGS = `pkg-config --cflags glfw3` -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL -L lib
INCLUDE = -I include

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	g++ $^ -o $(TARGET) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	g++ $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf build
	@rm -f $(TARGET)
