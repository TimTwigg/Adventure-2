@ECHO OFF
IF NOT EXIST build MKDIR build
CD build
cmake .. -G "MSYS Makefiles"
make
CD ..