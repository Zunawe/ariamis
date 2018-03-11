# Ariamis

This started as just following a tutorial on OpenGL 3 and has grown as I've learned more things. At the moment, I use it as a base for homework in my graphics class, and it suits testing a new concept or shader pretty well.

There is an `example.cpp` which shows how to use it, which is how I test it, and I try to keep up on documentation comments, but that's all there is right now.

## Dependencies

You'll need [glm](https://glm.g-truc.net/0.9.8/index.html) and [GLFW](http://www.glfw.org/). This was built on and for Ubuntu. The code should be portable, but the makefile isn't going to know what's going on on other platforms (probably).

## Classes

* **Camera** - Handles transformation of camera as if it were an object in the space. It can return you a view matrix.
* **Engine** - The glue and brains. This is what you interact with if you don't want to do any graphics programming, and is most of the interface.
* **Light** - Contains basic lighting attributes for colors and position. If the `w` component of the position is 0, it will be interpreted as a directional light and its direction will be what is specified in `position`. Light and its subclasses all map to the same struct in the default shader, but values of 0 in particular attributes indicate the type of light while in the shader (the classes take care of what needs to be set).
  * **PointLight** - Also has attenuation values.
  * **SpotLight** - Also has a direction and cone angle.
* **Material** - Similar to `Light`, but for object materials. It also holds textures for diffuse and specular lighting (and will eventually support normal mapping). 
* **Mesh** - Represents the shape of an object. All attribute data is stored in a vector of floats in the same order it will be passed into the vertex buffer object. The logic for accessing particular attributes and the positions of those attributes is covered by this class. Nothing here uses any OpenGL; it just holds the data to be passed to the GPU when you want to render something.
* **ObjectRenderer** - Describes how an object should be rendered. Each `ObjectRenderer` gets a `Mesh` and puts that mesh in a VAO. It also gets a list of materials to apply to the `Mesh`'s submeshes and a shader. An `ObjectRenderer` is responsible for drawing itself, and assumes the context is correctly set up.
* **Object** - An `Object` mostly handles transformations, but also has a function that gets called before every frame gets rendered.
* **Scene** - Contains `Object`s, `Light`s, and at least one `Camera`. You build a scene by adding these components and then tell the `Engine` to render it.
* **Shader** - Compiles and links shaders and can bind itself for use. Uniforms can be set here in a shorthand that fails gracefully and returns whether or not the uniform name was found.
* **Texture** - Handles loading an image from disk, sending it to the GPU, and saving the ID so it can be referenced later. This uses [stb_image](https://github.com/nothings/stb) for convenience.
