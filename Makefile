EXE = graphic
CFLAGS = `pkg-config --cflags glfw3` -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL

$(EXE): $(EXE).o shader_program.o mesh.o texture.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.cpp.o:
	g++ -c $(CFLAGS) $<

$(EXE).o: $(EXE).cpp $(EXE).h mesh.h texture.h
shader_program.o: shader_program.cpp shader_program.h
mesh.o: mesh.cpp mesh.h
texture.o: texture.cpp texture.h stb_image.h

clean:
	rm -f *.o
	rm -f $(EXE)