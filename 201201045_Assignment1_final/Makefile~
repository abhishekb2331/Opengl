CC = g++
CFLAGS = -Wall
PROG = cse251_sampleCode

SRCS = cse251_sampleCode.cpp
LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
