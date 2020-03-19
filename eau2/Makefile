build:
	g++ tests/test_serialize.cpp -o tests/test_serialize 
	g++ src/sorer/main.cpp -o tests/test_sorer
	mkdir data
	tests/file.py data/datafile.txt

test:
	tests/test_serialize.cpp
	tests/test_sorer -f data/datafile.txt
	

valgrind:

clean:
	rm tests/test_serialize
	rm tests/test_sorer
	rm data -r
