###############################################################################
# Name: Sarah Larkin
# Username: selarkin
# Program Assignment #3
# Due Date: 03/20/2020
# Filename: Makefile
#     Compiles thread.h, thread.cpp, and thread-main.cpp into executable prog3
#     using relative paths
###############################################################################
CC       = c++
FLAGS    = 
CFLAGS   = -g -O2 -Wno-write-strings -Wno-cpp -w
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/local/eit-linux/apps/ThreadMentor/include
TMLIB    = /local/eit-linux/apps/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV    = /local/eit-linux/apps/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = thread.o thread-main.o
EXE_FILE = prog3

${EXE_FILE}: ${OBJ_FILE}
	${CC} -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

thread.o: thread.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp

thread-main.o: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread
					
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
