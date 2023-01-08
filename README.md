# cpu-usage-tracker

There is a bug where cpu is below 0% after changing buffer to 1D instead of 3D, last working commit is aa62acd5eed298448abab43aed9256e7236bfd0c

Fixed at 24aea10ef0c7ce762103da813153f8697a6795df



SIGTERM:
"I compiled the example into sigterm-example. Starting it and sending a SIGTERM by using

$ kill PID

(replace PID with the actual process ID) results in the following output:



Makefile example

build:
    make TOOLCHAIN=[gcc, clang], where gcc is default

clean:
    make clean

test:
    make test
    TBD


