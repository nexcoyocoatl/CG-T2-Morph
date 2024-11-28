# Makefile para Windows

PROG = T2.exe
FONTES = main.cpp Objeto3D.cpp Ponto.cpp Ray.cpp TrianglePlane.cpp
OBJETOS = $(FONTES:.cpp=.o)
CPPFLAGS = -O3 -g -Iinclude # -Wall -g  # Todas as warnings, infos de debug
# Se estiver usando mingw64, troque lib por lib\x64 abaixo
LDFLAGS = -Llib -lfreeglut -lopengl32 -lglu32 -lm
CC = g++

$(PROG): $(OBJETOS)
	$(CC) $(CPPFLAGS) $(OBJETOS) -o $@ $(LDFLAGS)

clean:
	-@ del $(OBJETOS) $(PROG)
