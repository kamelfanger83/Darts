# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/linusl/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/223.8617.54/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/linusl/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/223.8617.54/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/linusl/Desktop/techlab/GLFW-CMake-starter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/GLFW-CMake-starter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GLFW-CMake-starter.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GLFW-CMake-starter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GLFW-CMake-starter.dir/flags.make

CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o: CMakeFiles/GLFW-CMake-starter.dir/flags.make
CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o: /home/linusl/Desktop/techlab/GLFW-CMake-starter/main.cpp
CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o: CMakeFiles/GLFW-CMake-starter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o -MF CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o.d -o CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o -c /home/linusl/Desktop/techlab/GLFW-CMake-starter/main.cpp

CMakeFiles/GLFW-CMake-starter.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GLFW-CMake-starter.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linusl/Desktop/techlab/GLFW-CMake-starter/main.cpp > CMakeFiles/GLFW-CMake-starter.dir/main.cpp.i

CMakeFiles/GLFW-CMake-starter.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GLFW-CMake-starter.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linusl/Desktop/techlab/GLFW-CMake-starter/main.cpp -o CMakeFiles/GLFW-CMake-starter.dir/main.cpp.s

# Object files for target GLFW-CMake-starter
GLFW__CMake__starter_OBJECTS = \
"CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o"

# External object files for target GLFW-CMake-starter
GLFW__CMake__starter_EXTERNAL_OBJECTS =

GLFW-CMake-starter: CMakeFiles/GLFW-CMake-starter.dir/main.cpp.o
GLFW-CMake-starter: CMakeFiles/GLFW-CMake-starter.dir/build.make
GLFW-CMake-starter: /usr/lib/libGL.so
GLFW-CMake-starter: /usr/lib/libGLU.so
GLFW-CMake-starter: glfw/src/libglfw3.a
GLFW-CMake-starter: /usr/lib/libGLEW.so
GLFW-CMake-starter: /usr/lib/librt.a
GLFW-CMake-starter: /usr/lib/libm.so
GLFW-CMake-starter: /usr/lib/libX11.so
GLFW-CMake-starter: CMakeFiles/GLFW-CMake-starter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GLFW-CMake-starter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GLFW-CMake-starter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GLFW-CMake-starter.dir/build: GLFW-CMake-starter
.PHONY : CMakeFiles/GLFW-CMake-starter.dir/build

CMakeFiles/GLFW-CMake-starter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GLFW-CMake-starter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GLFW-CMake-starter.dir/clean

CMakeFiles/GLFW-CMake-starter.dir/depend:
	cd /home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linusl/Desktop/techlab/GLFW-CMake-starter /home/linusl/Desktop/techlab/GLFW-CMake-starter /home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug /home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug /home/linusl/Desktop/techlab/GLFW-CMake-starter/cmake-build-debug/CMakeFiles/GLFW-CMake-starter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GLFW-CMake-starter.dir/depend

