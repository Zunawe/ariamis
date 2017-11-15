EXE = graphic
CFLAGS = `pkg-config --cflags glfw3` -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL

$(EXE): $(EXE).o shader_program.o mesh.o texture.o camera.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.cpp.o:
	g++ -c $(CFLAGS) $<

$(EXE).o: $(EXE).cpp $(EXE).hpp mesh.hpp texture.hpp camera.hpp
shader_program.o: shader_program.cpp shader_program.hpp
mesh.o: mesh.cpp mesh.hpp
texture.o: texture.cpp texture.hpp stb_image.h
camera.o: camera.cpp camera.hpp

clean:
	rm -f *.o
	rm -f $(EXE)