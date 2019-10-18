tracer: main.cpp vec3.cpp linear_algebra.cpp
	g++ main.cpp vec3.cpp linear_algebra.cpp -o tracer -Wall --std=c++14

test: tracer
	./tracer > out.ppm

clean:
	rm tracer
	rm *.ppm
