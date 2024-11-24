# Makefile para Linux e macOS

PROG = T2
FONTES = main.cpp Linha.cpp Objeto3D.cpp Ponto.cpp Ray.cpp TrianglePlane.cpp
OBJETOS = $(FONTES:.cpp=.o)
CPPFLAGS = -g -O3 -DGL_SILENCE_DEPRECATION -Wno-deprecated # -Wall -g  # Todas as warnings, infos de debug
CC = g++

UNAME = $(shell uname)

ifeq ($(UNAME),Darwin)
	LDFLAGS = -framework OpenGL -framework Cocoa -framework GLUT -lm
else
	LDFLAGS = -lGL -lGLU -lglut -lm
endif

$(PROG): $(OBJETOS)
	$(CC) $(CPPFLAGS) $(OBJETOS) $(LDFLAGS) -o $(PROG)

clean:
	-@ rm -f $(OBJETOS) $(PROG)
