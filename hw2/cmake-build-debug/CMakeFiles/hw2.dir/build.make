# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2017.3.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2017.3.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ds-ubuntu/Documents/Computer-Architecture/hw2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw2.dir/flags.make

CMakeFiles/hw2.dir/pa2.c.o: CMakeFiles/hw2.dir/flags.make
CMakeFiles/hw2.dir/pa2.c.o: ../pa2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw2.dir/pa2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw2.dir/pa2.c.o   -c /home/ds-ubuntu/Documents/Computer-Architecture/hw2/pa2.c

CMakeFiles/hw2.dir/pa2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw2.dir/pa2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ds-ubuntu/Documents/Computer-Architecture/hw2/pa2.c > CMakeFiles/hw2.dir/pa2.c.i

CMakeFiles/hw2.dir/pa2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw2.dir/pa2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ds-ubuntu/Documents/Computer-Architecture/hw2/pa2.c -o CMakeFiles/hw2.dir/pa2.c.s

CMakeFiles/hw2.dir/pa2.c.o.requires:

.PHONY : CMakeFiles/hw2.dir/pa2.c.o.requires

CMakeFiles/hw2.dir/pa2.c.o.provides: CMakeFiles/hw2.dir/pa2.c.o.requires
	$(MAKE) -f CMakeFiles/hw2.dir/build.make CMakeFiles/hw2.dir/pa2.c.o.provides.build
.PHONY : CMakeFiles/hw2.dir/pa2.c.o.provides

CMakeFiles/hw2.dir/pa2.c.o.provides.build: CMakeFiles/hw2.dir/pa2.c.o


CMakeFiles/hw2.dir/test.c.o: CMakeFiles/hw2.dir/flags.make
CMakeFiles/hw2.dir/test.c.o: ../test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hw2.dir/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hw2.dir/test.c.o   -c /home/ds-ubuntu/Documents/Computer-Architecture/hw2/test.c

CMakeFiles/hw2.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw2.dir/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ds-ubuntu/Documents/Computer-Architecture/hw2/test.c > CMakeFiles/hw2.dir/test.c.i

CMakeFiles/hw2.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw2.dir/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ds-ubuntu/Documents/Computer-Architecture/hw2/test.c -o CMakeFiles/hw2.dir/test.c.s

CMakeFiles/hw2.dir/test.c.o.requires:

.PHONY : CMakeFiles/hw2.dir/test.c.o.requires

CMakeFiles/hw2.dir/test.c.o.provides: CMakeFiles/hw2.dir/test.c.o.requires
	$(MAKE) -f CMakeFiles/hw2.dir/build.make CMakeFiles/hw2.dir/test.c.o.provides.build
.PHONY : CMakeFiles/hw2.dir/test.c.o.provides

CMakeFiles/hw2.dir/test.c.o.provides.build: CMakeFiles/hw2.dir/test.c.o


# Object files for target hw2
hw2_OBJECTS = \
"CMakeFiles/hw2.dir/pa2.c.o" \
"CMakeFiles/hw2.dir/test.c.o"

# External object files for target hw2
hw2_EXTERNAL_OBJECTS =

hw2: CMakeFiles/hw2.dir/pa2.c.o
hw2: CMakeFiles/hw2.dir/test.c.o
hw2: CMakeFiles/hw2.dir/build.make
hw2: CMakeFiles/hw2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable hw2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw2.dir/build: hw2

.PHONY : CMakeFiles/hw2.dir/build

CMakeFiles/hw2.dir/requires: CMakeFiles/hw2.dir/pa2.c.o.requires
CMakeFiles/hw2.dir/requires: CMakeFiles/hw2.dir/test.c.o.requires

.PHONY : CMakeFiles/hw2.dir/requires

CMakeFiles/hw2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw2.dir/clean

CMakeFiles/hw2.dir/depend:
	cd /home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ds-ubuntu/Documents/Computer-Architecture/hw2 /home/ds-ubuntu/Documents/Computer-Architecture/hw2 /home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug /home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug /home/ds-ubuntu/Documents/Computer-Architecture/hw2/cmake-build-debug/CMakeFiles/hw2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw2.dir/depend

