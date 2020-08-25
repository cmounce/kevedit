#!/bin/sh
# Tell make to include debug symbols without optimizations: ./configure CFLAGS="-g"

params=""

for arg in $@; do
    case $arg in
    "gdb")
        params="$params --vgdb=yes --vgdb-error=0"
        ;;
    "gen-sup")
        params="$params --gen-suppressions=yes"
        ;;
    *)
        echo "Usage: $0 [gdb] [gen-sup]"
        exit 1
        ;;
    esac
done

valgrind $params --suppressions=valgrind.supp --num-callers=40 --leak-check=full ./src/kevedit/kevedit
