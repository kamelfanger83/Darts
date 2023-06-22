This is based on this repo for the boilerplate glfw/cmake code: https://github.com/juliettef/GLFW-CMake-starter

You can tweak the parameters through the very sophistiaced method of editing the code starting from line 94.

I have noticed that the glfw code does not compile with some compilers due to encoding issues. It has worked on linux for me and on windows I used the Visual Studio compiler because MinGW did not work.

Also note that the framerate is adjusted to your screens max fps so you might need to lower the speeds and the angle steps.