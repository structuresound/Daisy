# $ make
# $ ./boids-sim

CC = g++
CFLAGS = -std=c++11 -c
DEBUG = -g
SFML = -lsfml-graphics -lsfml-window -lsfml-system
FRAMEWORKS = -framework OpenGL -framework GLUT
INCLUDE = -I../../imgui/ -I../../imgui-sfml/

all: boids

boids: main.o Game.o imgui-SFML.o
	$(CC) $(DEBUG) $(INCLUDE) imgui_draw.o imgui_tables.o imgui_widgets.o imgui.o imgui-SFML.o Game.o main.o -o boids-sim $(SFML) $(FRAMEWORKS)

Game.o: Game.cpp
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE) Game.cpp

imgui-SFML.o: imgui.o
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE) imgui-SFML.cpp

imgui.o: imgui.cpp
	$(CC) $(CFLAGS) $(DEBUG) $(INCLUDE) imgui_widgets.cpp imgui_tables.cpp imgui_draw.cpp imgui.cpp

clean:
	\rm *.o boids-sim

tar:
	tar cfv boids.tar Pvector.h Pvector.cpp Boid.h Boid.cpp \
		Flock.h Flock.cpp Game.cpp Game.h
