all:
	g++ main.cpp Shader.cpp object.cpp gameSimulation.cpp -o main -lglfw -lGL -lGLEW -lBox2D -Wall -std=c++11 
run:
	./main
