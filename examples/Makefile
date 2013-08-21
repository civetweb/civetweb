CFLAGS=	-W -Wall -I.. -pthread -g
LIB_SOURCE =  ../civetweb.c  ../md5.c 
all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl" ; \
	  $(CC) $(CFLAGS) hello.c $(LIB_SOURCE) $$LIBS $(ADD) -o hello;
	  $(CC) $(CFLAGS) upload.c $(LIB_SOURCE) $$LIBS $(ADD) -o upload;
	  $(CC) $(CFLAGS) post.c $(LIB_SOURCE) $$LIBS $(ADD) -o post;
	  $(CC) $(CFLAGS) -DUSE_WEBSOCKET websocket.c $(LIB_SOURCE) $$LIBS $(ADD) -o websocket;
	  $(CC) $(CFLAGS) chat.c $(LIB_SOURCE) $$LIBS $(ADD) -o chat


MSVC    = e:/vc6
CL      = $(MSVC)/bin/cl
CLFLAGS = /MD /TC /nologo $(DBG) /W3 /DNO_SSL \
        /I$(MSVC)/include /I.. /Dsnprintf=_snprintf \
        /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86

windows:
	$(CL) upload.c $(LIB_SOURCE) $(CLFLAGS)
	$(CL) hello.c $(LIB_SOURCE) $(CLFLAGS)
	$(CL) post.c $(LIB_SOURCE) $(CLFLAGS)
	$(CL) /DUSE_WEBSOCKET websocket.c $(LIB_SOURCE) $(CLFLAGS)

clean:
	rm -rf hello upload post websocket chat *.exe *.dSYM *.obj
