# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dpp/QueryHouse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dpp/QueryHouse/build_no_afl

# Include any dependencies generated for this target.
include third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/flags.make

third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o: third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/flags.make
third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o: ../third_party/abseil-cpp/absl/base/internal/scoped_set_env.cc
third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o: third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dpp/QueryHouse/build_no_afl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o"
	cd /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o -MF CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o.d -o CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o -c /home/dpp/QueryHouse/third_party/abseil-cpp/absl/base/internal/scoped_set_env.cc

third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.i"
	cd /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dpp/QueryHouse/third_party/abseil-cpp/absl/base/internal/scoped_set_env.cc > CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.i

third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.s"
	cd /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dpp/QueryHouse/third_party/abseil-cpp/absl/base/internal/scoped_set_env.cc -o CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.s

# Object files for target absl_scoped_set_env
absl_scoped_set_env_OBJECTS = \
"CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o"

# External object files for target absl_scoped_set_env
absl_scoped_set_env_EXTERNAL_OBJECTS =

third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so: third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/internal/scoped_set_env.cc.o
third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so: third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/build.make
third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so: third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.so
third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so: third_party/abseil-cpp/absl/base/libabsl_log_severity.so
third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so: third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dpp/QueryHouse/build_no_afl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libabsl_scoped_set_env.so"
	cd /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_scoped_set_env.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/build: third_party/abseil-cpp/absl/base/libabsl_scoped_set_env.so
.PHONY : third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/build

third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/clean:
	cd /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base && $(CMAKE_COMMAND) -P CMakeFiles/absl_scoped_set_env.dir/cmake_clean.cmake
.PHONY : third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/clean

third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/depend:
	cd /home/dpp/QueryHouse/build_no_afl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dpp/QueryHouse /home/dpp/QueryHouse/third_party/abseil-cpp/absl/base /home/dpp/QueryHouse/build_no_afl /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base /home/dpp/QueryHouse/build_no_afl/third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/abseil-cpp/absl/base/CMakeFiles/absl_scoped_set_env.dir/depend

