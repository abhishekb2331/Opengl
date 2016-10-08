CC = g++
CFLAGS = -Wall
PROG = motocross

SRCS = Assignment3_201201045_201201055.cpp vec3f.cpp imageloader.cpp
LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
