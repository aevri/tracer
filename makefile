tracer:
	gcc main.cpp -o tracer

test: tracer
	./tracer

clean:
	rm tracer
