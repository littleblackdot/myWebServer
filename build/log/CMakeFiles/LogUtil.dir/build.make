# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/little/little/Code_C++/myWebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/little/little/Code_C++/myWebServer/build

# Include any dependencies generated for this target.
include log/CMakeFiles/LogUtil.dir/depend.make

# Include the progress variables for this target.
include log/CMakeFiles/LogUtil.dir/progress.make

# Include the compile flags for this target's objects.
include log/CMakeFiles/LogUtil.dir/flags.make

log/CMakeFiles/LogUtil.dir/AsynLog.cpp.o: log/CMakeFiles/LogUtil.dir/flags.make
log/CMakeFiles/LogUtil.dir/AsynLog.cpp.o: ../log/AsynLog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object log/CMakeFiles/LogUtil.dir/AsynLog.cpp.o"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LogUtil.dir/AsynLog.cpp.o -c /home/little/little/Code_C++/myWebServer/log/AsynLog.cpp

log/CMakeFiles/LogUtil.dir/AsynLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogUtil.dir/AsynLog.cpp.i"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/little/little/Code_C++/myWebServer/log/AsynLog.cpp > CMakeFiles/LogUtil.dir/AsynLog.cpp.i

log/CMakeFiles/LogUtil.dir/AsynLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogUtil.dir/AsynLog.cpp.s"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/little/little/Code_C++/myWebServer/log/AsynLog.cpp -o CMakeFiles/LogUtil.dir/AsynLog.cpp.s

log/CMakeFiles/LogUtil.dir/FileUtil.cpp.o: log/CMakeFiles/LogUtil.dir/flags.make
log/CMakeFiles/LogUtil.dir/FileUtil.cpp.o: ../log/FileUtil.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object log/CMakeFiles/LogUtil.dir/FileUtil.cpp.o"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LogUtil.dir/FileUtil.cpp.o -c /home/little/little/Code_C++/myWebServer/log/FileUtil.cpp

log/CMakeFiles/LogUtil.dir/FileUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogUtil.dir/FileUtil.cpp.i"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/little/little/Code_C++/myWebServer/log/FileUtil.cpp > CMakeFiles/LogUtil.dir/FileUtil.cpp.i

log/CMakeFiles/LogUtil.dir/FileUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogUtil.dir/FileUtil.cpp.s"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/little/little/Code_C++/myWebServer/log/FileUtil.cpp -o CMakeFiles/LogUtil.dir/FileUtil.cpp.s

log/CMakeFiles/LogUtil.dir/Log.cpp.o: log/CMakeFiles/LogUtil.dir/flags.make
log/CMakeFiles/LogUtil.dir/Log.cpp.o: ../log/Log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object log/CMakeFiles/LogUtil.dir/Log.cpp.o"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LogUtil.dir/Log.cpp.o -c /home/little/little/Code_C++/myWebServer/log/Log.cpp

log/CMakeFiles/LogUtil.dir/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogUtil.dir/Log.cpp.i"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/little/little/Code_C++/myWebServer/log/Log.cpp > CMakeFiles/LogUtil.dir/Log.cpp.i

log/CMakeFiles/LogUtil.dir/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogUtil.dir/Log.cpp.s"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/little/little/Code_C++/myWebServer/log/Log.cpp -o CMakeFiles/LogUtil.dir/Log.cpp.s

log/CMakeFiles/LogUtil.dir/LogFile.cpp.o: log/CMakeFiles/LogUtil.dir/flags.make
log/CMakeFiles/LogUtil.dir/LogFile.cpp.o: ../log/LogFile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object log/CMakeFiles/LogUtil.dir/LogFile.cpp.o"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LogUtil.dir/LogFile.cpp.o -c /home/little/little/Code_C++/myWebServer/log/LogFile.cpp

log/CMakeFiles/LogUtil.dir/LogFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogUtil.dir/LogFile.cpp.i"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/little/little/Code_C++/myWebServer/log/LogFile.cpp > CMakeFiles/LogUtil.dir/LogFile.cpp.i

log/CMakeFiles/LogUtil.dir/LogFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogUtil.dir/LogFile.cpp.s"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/little/little/Code_C++/myWebServer/log/LogFile.cpp -o CMakeFiles/LogUtil.dir/LogFile.cpp.s

log/CMakeFiles/LogUtil.dir/LogStream.cpp.o: log/CMakeFiles/LogUtil.dir/flags.make
log/CMakeFiles/LogUtil.dir/LogStream.cpp.o: ../log/LogStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object log/CMakeFiles/LogUtil.dir/LogStream.cpp.o"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LogUtil.dir/LogStream.cpp.o -c /home/little/little/Code_C++/myWebServer/log/LogStream.cpp

log/CMakeFiles/LogUtil.dir/LogStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogUtil.dir/LogStream.cpp.i"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/little/little/Code_C++/myWebServer/log/LogStream.cpp > CMakeFiles/LogUtil.dir/LogStream.cpp.i

log/CMakeFiles/LogUtil.dir/LogStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogUtil.dir/LogStream.cpp.s"
	cd /home/little/little/Code_C++/myWebServer/build/log && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/little/little/Code_C++/myWebServer/log/LogStream.cpp -o CMakeFiles/LogUtil.dir/LogStream.cpp.s

# Object files for target LogUtil
LogUtil_OBJECTS = \
"CMakeFiles/LogUtil.dir/AsynLog.cpp.o" \
"CMakeFiles/LogUtil.dir/FileUtil.cpp.o" \
"CMakeFiles/LogUtil.dir/Log.cpp.o" \
"CMakeFiles/LogUtil.dir/LogFile.cpp.o" \
"CMakeFiles/LogUtil.dir/LogStream.cpp.o"

# External object files for target LogUtil
LogUtil_EXTERNAL_OBJECTS =

log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/AsynLog.cpp.o
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/FileUtil.cpp.o
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/Log.cpp.o
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/LogFile.cpp.o
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/LogStream.cpp.o
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/build.make
log/libLogUtil.a: log/CMakeFiles/LogUtil.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/little/little/Code_C++/myWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libLogUtil.a"
	cd /home/little/little/Code_C++/myWebServer/build/log && $(CMAKE_COMMAND) -P CMakeFiles/LogUtil.dir/cmake_clean_target.cmake
	cd /home/little/little/Code_C++/myWebServer/build/log && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LogUtil.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
log/CMakeFiles/LogUtil.dir/build: log/libLogUtil.a

.PHONY : log/CMakeFiles/LogUtil.dir/build

log/CMakeFiles/LogUtil.dir/clean:
	cd /home/little/little/Code_C++/myWebServer/build/log && $(CMAKE_COMMAND) -P CMakeFiles/LogUtil.dir/cmake_clean.cmake
.PHONY : log/CMakeFiles/LogUtil.dir/clean

log/CMakeFiles/LogUtil.dir/depend:
	cd /home/little/little/Code_C++/myWebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/little/little/Code_C++/myWebServer /home/little/little/Code_C++/myWebServer/log /home/little/little/Code_C++/myWebServer/build /home/little/little/Code_C++/myWebServer/build/log /home/little/little/Code_C++/myWebServer/build/log/CMakeFiles/LogUtil.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : log/CMakeFiles/LogUtil.dir/depend

