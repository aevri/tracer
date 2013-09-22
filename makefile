tracer: main.cpp
	g++ main.cpp -o tracer -Wall

test: tracer
	./tracer > out.ppm

clean:
	rm tracer
	rm *.ppm
