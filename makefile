tracer: main.cpp
	g++ main.cpp -o tracer -Wall --std=c++14

test: tracer
	./tracer > out.ppm

clean:
	rm tracer
	rm *.ppm
