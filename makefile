tracer: main.cpp
	g++ main.cpp -o tracer -Wall --std=c++11

test: tracer
	./tracer > out.ppm

clean:
	rm tracer
	rm *.ppm
