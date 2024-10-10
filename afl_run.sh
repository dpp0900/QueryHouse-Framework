#!/bin/bash
AFL_DEBUG=1 __AFL_SHM_ID=1234 /usr/local/mysql/bin/mysqld 2>&1 | grep "__afl_map_size" | tail -n 1 | cut -d"," -f8 | cut -d" " -f 3 > /tmp/mapsize
AFL_IGNORE_PROBLEMS=1 AFL_MAP_SIZE=$(cat /tmp/mapsize) python3 run.py mysql data/fuzz_root/mysql_input/