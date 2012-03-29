# add custom variables to this file

# OF_ROOT allows to move projects outside apps/* just set this variable to the
# absoulte path to the OF root folder

OF_ROOT = ../../..


# USER_CFLAGS allows to pass custom flags to the compiler
# for example search paths like:
# USER_CFLAGS = -I src/objects

USER_CFLAGS = -I/home/walter/openframeworks007/addons/ofxOpenSceneGraph/lib/linux/include -shared


# USER_LDFLAGS allows to pass custom flags to the linker
# for example libraries like:
# USER_LDFLAGS = libs/libawesomelib.a

USER_LDFLAGS =-L/home/walter/openframeworks007/addons/ofxOpenSceneGraph/lib/linux/lib --export-dynamic


# use this to add system libraries for example:
# USER_LIBS = -lpango

USER_LIBS = -ldl -lm -ljack -losg -losgViewer



# change this to add different compiler optimizations to your project

USER_COMPILER_OPTIMIZATION = -march=native -mtune=native -Os