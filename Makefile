CC = clang
CFLAGS = -isysroot /var/mobile/Documents/Development/SDKs/iPhoneOS13.4.1.sdk -g -O0 -lpthread
#-Wno-deprecated -Wno-deprecated-declarations
INCLUDES = 
OBJS = 
MAINOBJS = main.o
LIBS = 

all: exe splitter.dylib

exe: ${MAINOBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${MAINOBJS} ${LIBS}
	chmod +x $@

clean:
	-rm -f *.o

splitter.dylib: splitter.c
	${CC} ${CFLAGS} ${INCLUDES} -dynamiclib splitter.c -o splitter.dylib

main.o: main.c
	${CC} ${CFLAGS} ${INCLUDES} -c main.c


