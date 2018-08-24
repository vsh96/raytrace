all: vec3.hpp ray.hpp
	g++ main.cpp -o raytrace

run:
	raytrace