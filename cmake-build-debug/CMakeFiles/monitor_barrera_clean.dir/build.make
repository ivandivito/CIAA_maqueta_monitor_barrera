# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/ivan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/172.3968.17/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ivan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/172.3968.17/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ivan/Documents/CIAA/maqueta_monitor_barrera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ivan/Documents/CIAA/maqueta_monitor_barrera/cmake-build-debug

# Utility rule file for monitor_barrera_clean.

# Include the progress variables for this target.
include CMakeFiles/monitor_barrera_clean.dir/progress.make

CMakeFiles/monitor_barrera_clean:
	cd /home/ivan/Documents/CIAA/maqueta_monitor_barrera/firmware && make clean

monitor_barrera_clean: CMakeFiles/monitor_barrera_clean
monitor_barrera_clean: CMakeFiles/monitor_barrera_clean.dir/build.make

.PHONY : monitor_barrera_clean

# Rule to build all files generated by this target.
CMakeFiles/monitor_barrera_clean.dir/build: monitor_barrera_clean

.PHONY : CMakeFiles/monitor_barrera_clean.dir/build

CMakeFiles/monitor_barrera_clean.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/monitor_barrera_clean.dir/cmake_clean.cmake
.PHONY : CMakeFiles/monitor_barrera_clean.dir/clean

CMakeFiles/monitor_barrera_clean.dir/depend:
	cd /home/ivan/Documents/CIAA/maqueta_monitor_barrera/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ivan/Documents/CIAA/maqueta_monitor_barrera /home/ivan/Documents/CIAA/maqueta_monitor_barrera /home/ivan/Documents/CIAA/maqueta_monitor_barrera/cmake-build-debug /home/ivan/Documents/CIAA/maqueta_monitor_barrera/cmake-build-debug /home/ivan/Documents/CIAA/maqueta_monitor_barrera/cmake-build-debug/CMakeFiles/monitor_barrera_clean.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/monitor_barrera_clean.dir/depend
