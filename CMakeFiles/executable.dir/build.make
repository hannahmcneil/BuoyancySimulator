# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/leehagaman/Desktop/cs184/BuoyancySimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/leehagaman/Desktop/cs184/BuoyancySimulator

# Include any dependencies generated for this target.
include CMakeFiles/executable.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/executable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/executable.dir/flags.make

CMakeFiles/executable.dir/main.cpp.o: CMakeFiles/executable.dir/flags.make
CMakeFiles/executable.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/executable.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/executable.dir/main.cpp.o -c /Users/leehagaman/Desktop/cs184/BuoyancySimulator/main.cpp

CMakeFiles/executable.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/executable.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leehagaman/Desktop/cs184/BuoyancySimulator/main.cpp > CMakeFiles/executable.dir/main.cpp.i

CMakeFiles/executable.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/executable.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leehagaman/Desktop/cs184/BuoyancySimulator/main.cpp -o CMakeFiles/executable.dir/main.cpp.s

CMakeFiles/executable.dir/meshHandler.cpp.o: CMakeFiles/executable.dir/flags.make
CMakeFiles/executable.dir/meshHandler.cpp.o: meshHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/executable.dir/meshHandler.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/executable.dir/meshHandler.cpp.o -c /Users/leehagaman/Desktop/cs184/BuoyancySimulator/meshHandler.cpp

CMakeFiles/executable.dir/meshHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/executable.dir/meshHandler.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leehagaman/Desktop/cs184/BuoyancySimulator/meshHandler.cpp > CMakeFiles/executable.dir/meshHandler.cpp.i

CMakeFiles/executable.dir/meshHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/executable.dir/meshHandler.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leehagaman/Desktop/cs184/BuoyancySimulator/meshHandler.cpp -o CMakeFiles/executable.dir/meshHandler.cpp.s

CMakeFiles/executable.dir/simulate.cpp.o: CMakeFiles/executable.dir/flags.make
CMakeFiles/executable.dir/simulate.cpp.o: simulate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/executable.dir/simulate.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/executable.dir/simulate.cpp.o -c /Users/leehagaman/Desktop/cs184/BuoyancySimulator/simulate.cpp

CMakeFiles/executable.dir/simulate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/executable.dir/simulate.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leehagaman/Desktop/cs184/BuoyancySimulator/simulate.cpp > CMakeFiles/executable.dir/simulate.cpp.i

CMakeFiles/executable.dir/simulate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/executable.dir/simulate.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leehagaman/Desktop/cs184/BuoyancySimulator/simulate.cpp -o CMakeFiles/executable.dir/simulate.cpp.s

CMakeFiles/executable.dir/waterPoint.cpp.o: CMakeFiles/executable.dir/flags.make
CMakeFiles/executable.dir/waterPoint.cpp.o: waterPoint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/executable.dir/waterPoint.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/executable.dir/waterPoint.cpp.o -c /Users/leehagaman/Desktop/cs184/BuoyancySimulator/waterPoint.cpp

CMakeFiles/executable.dir/waterPoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/executable.dir/waterPoint.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/leehagaman/Desktop/cs184/BuoyancySimulator/waterPoint.cpp > CMakeFiles/executable.dir/waterPoint.cpp.i

CMakeFiles/executable.dir/waterPoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/executable.dir/waterPoint.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/leehagaman/Desktop/cs184/BuoyancySimulator/waterPoint.cpp -o CMakeFiles/executable.dir/waterPoint.cpp.s

# Object files for target executable
executable_OBJECTS = \
"CMakeFiles/executable.dir/main.cpp.o" \
"CMakeFiles/executable.dir/meshHandler.cpp.o" \
"CMakeFiles/executable.dir/simulate.cpp.o" \
"CMakeFiles/executable.dir/waterPoint.cpp.o"

# External object files for target executable
executable_EXTERNAL_OBJECTS =

executable: CMakeFiles/executable.dir/main.cpp.o
executable: CMakeFiles/executable.dir/meshHandler.cpp.o
executable: CMakeFiles/executable.dir/simulate.cpp.o
executable: CMakeFiles/executable.dir/waterPoint.cpp.o
executable: CMakeFiles/executable.dir/build.make
executable: CMakeFiles/executable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable executable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/executable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/executable.dir/build: executable

.PHONY : CMakeFiles/executable.dir/build

CMakeFiles/executable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/executable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/executable.dir/clean

CMakeFiles/executable.dir/depend:
	cd /Users/leehagaman/Desktop/cs184/BuoyancySimulator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/leehagaman/Desktop/cs184/BuoyancySimulator /Users/leehagaman/Desktop/cs184/BuoyancySimulator /Users/leehagaman/Desktop/cs184/BuoyancySimulator /Users/leehagaman/Desktop/cs184/BuoyancySimulator /Users/leehagaman/Desktop/cs184/BuoyancySimulator/CMakeFiles/executable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/executable.dir/depend

