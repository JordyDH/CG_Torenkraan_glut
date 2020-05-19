#-*-makefile-*-

TARGETS = \
    sandbox

all All: $(TARGETS)

################################################################################
INCPATH = -I/usr/local/include
LIBPATH = -L. -L/lib/x86_64-linux-gnu
LIBS = -lglut -lGLU -lGL -lGLw -lXt -lXext -lX11 -ljpeg -lm 
DEBUGFLAGS = -Ofast -g
CFLAGS = -DD32 $(DEBUGFLAGS) $(INCPATH) 
SRCS = \
	sandbox.c glutGameCore.c glutGameObjects.c glutGameCamera.c glutGameControl.c glutGameRender.c glutGameDebug.c InitJPG.c

OBJS = $(SRCS:.c=.o)

.SUFFIXES:	.c

.c.o:	
	cc -c $< $(CFLAGS)


InitJPG:  clean $(OBJS)
	LDOPTS= \
	cc -o InitJPG $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameCore:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameCore $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameControl:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameControl $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameRender:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameRender $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameObjects:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameRender $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameCamera:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameCamera $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

glutGameDebug:  clean $(OBJS)
	LDOPTS= \
	cc -o glutGameCamera $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

sandbox:  clean $(OBJS)
	LDOPTS= \
	cc -o sandbox $(CFLAGS) $(OBJS) $(LIBPATH) $(LIBS) 

clean:
	rm -f sandbox glutGameCore glutGameControl glutGameObjects glutGameRender glutGameCamera glutGameDebug InitJPG $(OBJS) core

print:
	listing sandbox.h $(SRCS)
