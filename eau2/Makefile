build:
	g++ -std=c++11 -w tests/test_serialize.cpp -o tests/test_serialize 
	g++ -std=c++11 -w src/sorer/main.cpp -o tests/test_sorer
	g++ -std=c++11 -w tests/kv_test.cpp -o tests/kv_test
	mkdir data
	tests/file.py data/datafile.txt

test:
	tests/kv_test.cpp
	tests/test_serialize.cpp
	tests/test_sorer -f data/datafile.txt
	

valgrind:

clean:
	rm tests/test_serialize
	rm tests/test_sorer
	rm data -r
