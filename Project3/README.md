# Before
Make sure you have installed libcsv and libfuse
Write terminal following command:

    export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib

# Compiling

    gcc main.c -o main -D_FILE_OFFSET_BITS=64 -lfuse -lcsv

# After

Create a test folder

If you dont want to debug 

    ./main test

If you want to debug

    ./main -d test