EXE = graphic
CFLAGS = `pkg-config --cflags glfw3` -std=c++11
LIBS = `pkg-config --static --libs glfw3` -lGL

$(EXE): $(EXE).o context_manager.o libwilsonengine.a
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

libwilsonengine.a: shader.o mesh.o texture.o camera.o object_renderer.o obj_loader.o stb_image.o material.o light.o
	ar -cqv $@ $^

.cpp.o:
	g++ -c $(CFLAGS) $<

$(EXE).o: $(EXE).cpp $(EXE).hpp context_manager.hpp
context_manager.o: context_manager.cpp context_manager.hpp shader.hpp mesh.hpp texture.hpp camera.hpp object_renderer.hpp obj_loader.hpp stb_image.h

shader.o: shader.cpp shader.hpp
mesh.o: mesh.cpp mesh.hpp
texture.o: texture.cpp texture.hpp stb_image.h
camera.o: camera.cpp camera.hpp
object_renderer.o: object_renderer.cpp object_renderer.hpp mesh.hpp texture.hpp shader.hpp camera.hpp material.hpp
obj_loader.o: obj_loader.cpp obj_loader.hpp material.hpp
material.o: material.cpp material.hpp
light.o: light.cpp light.hpp

stb_image.o: stb_image.cpp stb_image.h

clean:
	rm -f *.o
	rm -f $(EXE)
	rm -f *.a
