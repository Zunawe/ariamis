EXE = graphic
CFLAGS = `pkg-config --cflags glfw3` -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL

$(EXE): $(EXE).o shader_program.o mesh.o texture.o object_renderer.o camera.o picopng.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.cpp.o:
	g++ -c $(CFLAGS) $<

$(EXE).o: $(EXE).cpp $(EXE).hpp object_renderer.hpp texture.hpp camera.hpp
shader_program.o: shader_program.cpp shader_program.hpp
mesh.o: mesh.cpp mesh.hpp
texture.o: texture.cpp texture.hpp picopng.hpp
camera.o: camera.cpp camera.hpp
object_renderer.o: object_renderer.cpp object_renderer.hpp mesh.hpp texture.hpp shader_program.hpp

picopng.o: picopng.cpp picopng.hpp

clean:
	rm -f *.o
	rm -f $(EXE)