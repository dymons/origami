# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /home/dymons/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.6668.126/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/dymons/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.6668.126/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dymons/Downloads/study/origami

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dymons/Downloads/study/origami/cmake-build-debug

# Include any dependencies generated for this target.
include origami_test/CMakeFiles/character_sets_test.dir/depend.make

# Include the progress variables for this target.
include origami_test/CMakeFiles/character_sets_test.dir/progress.make

# Include the compile flags for this target's objects.
include origami_test/CMakeFiles/character_sets_test.dir/flags.make

origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o: origami_test/CMakeFiles/character_sets_test.dir/flags.make
origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o: ../origami_test/character_sets_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dymons/Downloads/study/origami/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o"
	cd /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o -c /home/dymons/Downloads/study/origami/origami_test/character_sets_test.cpp

origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/character_sets_test.dir/character_sets_test.cpp.i"
	cd /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dymons/Downloads/study/origami/origami_test/character_sets_test.cpp > CMakeFiles/character_sets_test.dir/character_sets_test.cpp.i

origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/character_sets_test.dir/character_sets_test.cpp.s"
	cd /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dymons/Downloads/study/origami/origami_test/character_sets_test.cpp -o CMakeFiles/character_sets_test.dir/character_sets_test.cpp.s

# Object files for target character_sets_test
character_sets_test_OBJECTS = \
"CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o"

# External object files for target character_sets_test
character_sets_test_EXTERNAL_OBJECTS =

origami_test/character_sets_test: origami_test/CMakeFiles/character_sets_test.dir/character_sets_test.cpp.o
origami_test/character_sets_test: origami_test/CMakeFiles/character_sets_test.dir/build.make
origami_test/character_sets_test: gtest/libgtest.a
origami_test/character_sets_test: origami_lexical/liborigami_lexical.a
origami_test/character_sets_test: origami_test/CMakeFiles/character_sets_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dymons/Downloads/study/origami/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable character_sets_test"
	cd /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/character_sets_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
origami_test/CMakeFiles/character_sets_test.dir/build: origami_test/character_sets_test

.PHONY : origami_test/CMakeFiles/character_sets_test.dir/build

origami_test/CMakeFiles/character_sets_test.dir/clean:
	cd /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test && $(CMAKE_COMMAND) -P CMakeFiles/character_sets_test.dir/cmake_clean.cmake
.PHONY : origami_test/CMakeFiles/character_sets_test.dir/clean

origami_test/CMakeFiles/character_sets_test.dir/depend:
	cd /home/dymons/Downloads/study/origami/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dymons/Downloads/study/origami /home/dymons/Downloads/study/origami/origami_test /home/dymons/Downloads/study/origami/cmake-build-debug /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test /home/dymons/Downloads/study/origami/cmake-build-debug/origami_test/CMakeFiles/character_sets_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : origami_test/CMakeFiles/character_sets_test.dir/depend

