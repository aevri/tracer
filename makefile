tracer: main.cpp
	gcc main.cpp -o tracer

test: tracer
	./tracer > out.ppm

clean:
	rm tracer
	rm *.ppm
