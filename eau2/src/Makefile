build:
	g++ -std=c++11 bench.cpp -o bench -w

run:
	{ \
	tar -xvfz datafile.tar.gz ; \
	bench datafile ; \
	}

clean:
	rm bench