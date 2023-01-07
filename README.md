# cpu-usage-tracker

SIGTERM:
"I compiled the example into sigterm-example. Starting it and sending a SIGTERM by using

$ kill PID

(replace PID with the actual process ID) results in the following output:

$ ./sigterm-example
Finished loop run 0.
Finished loop run 1.
Finished loop run 2.
Loop run was interrupted with 2s to go, finishing...
Finished loop run 3.
Done."


Makefile example#

build:
    make TOOLCHAIN=[gcc, clang], where gcc is default

clean:
    make clean

test:
    make test
