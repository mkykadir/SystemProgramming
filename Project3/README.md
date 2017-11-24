# Before
Make sure you have installed libcsv and libfuse
Write terminal following command:

    export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib

# Compiling

    gcc hello.c -o hello -D_FILE_OFFSET_BITS=64 -lfuse -lcsv