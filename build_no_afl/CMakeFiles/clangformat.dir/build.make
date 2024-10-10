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

# Utility rule file for clangformat.

# Include any custom commands dependencies for this target.
include CMakeFiles/clangformat.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clangformat.dir/progress.make

CMakeFiles/clangformat:
	clang-format -style=file -i /home/dpp/QueryHouse/srcs/custom_mutator.cc /home/dpp/QueryHouse/srcs/db.h /home/dpp/QueryHouse/srcs/db_driver.cc /home/dpp/QueryHouse/srcs/db_factory.cc /home/dpp/QueryHouse/srcs/internal/client/client.cc /home/dpp/QueryHouse/srcs/internal/client/client.h /home/dpp/QueryHouse/srcs/internal/client/client_mysql.cc /home/dpp/QueryHouse/srcs/internal/client/client_mysql.h /home/dpp/QueryHouse/srcs/internal/client/client_oracle.cc /home/dpp/QueryHouse/srcs/internal/client/client_oracle.h /home/dpp/QueryHouse/srcs/internal/client/client_postgresql.cc /home/dpp/QueryHouse/srcs/internal/client/client_postgresql.h /home/dpp/QueryHouse/srcs/internal/client/client_sqlite.cc /home/dpp/QueryHouse/srcs/internal/client/client_sqlite.h /home/dpp/QueryHouse/srcs/internal/client/test_client.cc /home/dpp/QueryHouse/srcs/internal/mysql/include/ast.h /home/dpp/QueryHouse/srcs/internal/mysql/include/define.h /home/dpp/QueryHouse/srcs/internal/mysql/include/mutator.h /home/dpp/QueryHouse/srcs/internal/mysql/include/utils.h /home/dpp/QueryHouse/srcs/internal/mysql/mysql.cc /home/dpp/QueryHouse/srcs/internal/mysql/mysql.h /home/dpp/QueryHouse/srcs/internal/mysql/parser/bison_parser.cpp /home/dpp/QueryHouse/srcs/internal/mysql/parser/bison_parser.h /home/dpp/QueryHouse/srcs/internal/mysql/parser/flex_lexer.cpp /home/dpp/QueryHouse/srcs/internal/mysql/parser/flex_lexer.h /home/dpp/QueryHouse/srcs/internal/mysql/parser/parser_typedef.h /home/dpp/QueryHouse/srcs/internal/mysql/srcs/ast.cpp /home/dpp/QueryHouse/srcs/internal/mysql/srcs/mutator.cpp /home/dpp/QueryHouse/srcs/internal/mysql/srcs/utils.cpp /home/dpp/QueryHouse/srcs/internal/oracle/mutator.h /home/dpp/QueryHouse/srcs/internal/oracle/oracle.cc /home/dpp/QueryHouse/srcs/internal/oracle/oracle.h /home/dpp/QueryHouse/srcs/internal/oracle/parser/bison_parser.cpp /home/dpp/QueryHouse/srcs/internal/oracle/parser/flex_lexer.cpp /home/dpp/QueryHouse/srcs/internal/oracle/srcs/ast.cpp /home/dpp/QueryHouse/srcs/internal/oracle/srcs/mutator.cpp /home/dpp/QueryHouse/srcs/internal/oracle/srcs/utils.cpp /home/dpp/QueryHouse/srcs/internal/postgresql/include/ast.h /home/dpp/QueryHouse/srcs/internal/postgresql/include/define.h /home/dpp/QueryHouse/srcs/internal/postgresql/include/mutator.h /home/dpp/QueryHouse/srcs/internal/postgresql/include/utils.h /home/dpp/QueryHouse/srcs/internal/postgresql/parser/bison_parser.cpp /home/dpp/QueryHouse/srcs/internal/postgresql/parser/bison_parser.h /home/dpp/QueryHouse/srcs/internal/postgresql/parser/flex_lexer.cpp /home/dpp/QueryHouse/srcs/internal/postgresql/parser/flex_lexer.h /home/dpp/QueryHouse/srcs/internal/postgresql/parser/parser_typedef.h /home/dpp/QueryHouse/srcs/internal/postgresql/postgresql.cc /home/dpp/QueryHouse/srcs/internal/postgresql/postgresql.h /home/dpp/QueryHouse/srcs/internal/postgresql/srcs/ast.cpp /home/dpp/QueryHouse/srcs/internal/postgresql/srcs/mutator.cpp /home/dpp/QueryHouse/srcs/internal/postgresql/srcs/utils.cpp /home/dpp/QueryHouse/srcs/internal/sqlite/include/ast.h /home/dpp/QueryHouse/srcs/internal/sqlite/include/define.h /home/dpp/QueryHouse/srcs/internal/sqlite/include/mutator.h /home/dpp/QueryHouse/srcs/internal/sqlite/include/utils.h /home/dpp/QueryHouse/srcs/internal/sqlite/parser/bison_parser.cpp /home/dpp/QueryHouse/srcs/internal/sqlite/parser/bison_parser.h /home/dpp/QueryHouse/srcs/internal/sqlite/parser/flex_lexer.cpp /home/dpp/QueryHouse/srcs/internal/sqlite/parser/flex_lexer.h /home/dpp/QueryHouse/srcs/internal/sqlite/parser/parser_typedef.h /home/dpp/QueryHouse/srcs/internal/sqlite/sqlite.cc /home/dpp/QueryHouse/srcs/internal/sqlite/sqlite.h /home/dpp/QueryHouse/srcs/internal/sqlite/srcs/ast.cpp /home/dpp/QueryHouse/srcs/internal/sqlite/srcs/mutator.cpp /home/dpp/QueryHouse/srcs/internal/sqlite/srcs/utils.cpp /home/dpp/QueryHouse/srcs/utils/config_validate.cc /home/dpp/QueryHouse/srcs/utils/config_validate.h

clangformat: CMakeFiles/clangformat
clangformat: CMakeFiles/clangformat.dir/build.make
.PHONY : clangformat

# Rule to build all files generated by this target.
CMakeFiles/clangformat.dir/build: clangformat
.PHONY : CMakeFiles/clangformat.dir/build

CMakeFiles/clangformat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clangformat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clangformat.dir/clean

CMakeFiles/clangformat.dir/depend:
	cd /home/dpp/QueryHouse/build_no_afl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dpp/QueryHouse /home/dpp/QueryHouse /home/dpp/QueryHouse/build_no_afl /home/dpp/QueryHouse/build_no_afl /home/dpp/QueryHouse/build_no_afl/CMakeFiles/clangformat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clangformat.dir/depend

