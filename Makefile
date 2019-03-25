all: ray.hpp
	g++ main.cpp -o raytrace

run:
	./raytrace