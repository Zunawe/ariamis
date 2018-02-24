# Graphics Engine

This started as just following a tutorial on OpenGL 3 and has grown as I've learned more things. At the moment, I use it as a base for homework in my graphics class, and it suits testing a new concept or shader pretty well.

## Dependencies

You'll need [glm](https://glm.g-truc.net/0.9.8/index.html) and [GLFW](http://www.glfw.org/). It's possible that the program might compile with just a `make` command after installing those. If not, look at the output of `make`; I haven't experimented with running this on other platforms (I use Ubuntu 16.04). I can't guarantee anything on Windows, but it's just C++ code and a couple libraries.

## Classes

* **Camera** - Handles transformation of camera as if it were an object in the space. It can return you a view matrix.
* **ContextManager** - A bad name for the thing that collects all the other classes, defines a display function, and loops over it. This is where you would build your own scenes and play around with objects.
* **Light** - Just a struct that I call a class right now, but could need methods eventually. Contains light attributes like position, diffuse color, etc...
* **Material** - Similar to `Light`, but for object materials. It also holds textures for diffuse and specular lighting (and will eventually support normal mapping). 
* **Mesh** - Represents the shape of an object. All attribute data is stored in a vector of floats in the same order it will be passed into the vertex buffer object. The logic for accessing particular attributes and the positions of those attributes is covered by this class. Nothing here uses any OpenGL; it just holds the data to be passed to the GPU when you want to render something.
* **ObjectRenderer** - Describes how an object should be rendered. Each `ObjectRenderer` gets a `Mesh` and puts that mesh in a VAO. It also gets a list of materials to apply to the `Mesh`'s submeshes and a shader. An `ObjectRenderer` is responsible for drawing itself, and assumes the context is correctly set up.
* **Shader** - Compiles and links shaders and can bind itself for use. Uniforms can be set here in a shorthand that fails gracefully and returns whether or not the uniform name was found.
* **Texture** - Handles loading an image from disk, sending it to the GPU, and saving the ID so it can be referenced later. This uses [stb_image](https://github.com/nothings/stb) for convenience.
