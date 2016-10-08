CC = g++
CFLAGS = -Wall
PROG = 201201045

SRCS = 201201045.cpp imageloader.cpp vec3f.cpp
LIBS = -lglut -lGL -lGLU -g

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
