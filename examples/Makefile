CFLAGS=	-W -Wall -I.. -pthread -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux && LIBS="-ldl" ; \
	  $(CC) $(CFLAGS) hello.c ../civetweb.c  $$LIBS $(ADD) -o hello;
	  $(CC) $(CFLAGS) upload.c ../civetweb.c  $$LIBS $(ADD) -o upload;
	  $(CC) $(CFLAGS) post.c ../civetweb.c  $$LIBS $(ADD) -o post;
	  $(CC) $(CFLAGS) -DUSE_WEBSOCKET websocket.c ../civetweb.c  $$LIBS $(ADD) -o websocket;
	  $(CC) $(CFLAGS) chat.c ../civetweb.c  $$LIBS $(ADD) -o chat


MSVC    = e:/vc6
CL      = $(MSVC)/bin/cl
CLFLAGS = /MD /TC /nologo $(DBG) /W3 /DNO_SSL \
        /I$(MSVC)/include /I.. /Dsnprintf=_snprintf \
        /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86

windows:
	$(CL) upload.c ../civetweb.c $(CLFLAGS)
	$(CL) hello.c ../civetweb.c $(CLFLAGS)
	$(CL) upload.c ../civetweb.c $(CLFLAGS)
	$(CL) post.c ../civetweb.c $(CLFLAGS)
	$(CL) post.c ../civetweb.c $(CLFLAGS)
	$(CL) /DUSE_WEBSOCKET websocket.c ../civetweb.c $(CLFLAGS)

clean:
	rm -rf hello upload post websocket chat *.exe *.dSYM *.obj
