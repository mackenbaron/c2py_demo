CC = gcc
CFLAGS = -g -c -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 -fno-strict-aliasing -fno-common -dynamic -arch i386 -arch x86_64 -g -Os -pipe -fno-common -fno-strict-aliasing -fwrapv -DENABLE_DTRACE -DMACOSX -DNDEBUG -Wall -Wstrict-prototypes -Wshorten-64-to-32 -DNDEBUG -g -fwrapv -Os -Wall -Wstrict-prototypes -DENABLE_DTRACE
LDFLAGS = -L/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -lpython2.7 -ldl -framework CoreFoundation

ALL: call

call: wrapper.o
	$(CC) $(LDFLAGS) wrapper.o -o call

wrapper.o: wrapper.c wrapper.h
	$(CC) $(CFLAGS) wrapper.c -o wrapper_tmp.o
	lipo wrapper_tmp.o -thin x86_64 -output wrapper.o

clean: 
	rm *.o call