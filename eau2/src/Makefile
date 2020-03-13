run: dircheck
	- make first
	- make second

first:
	docker build -t cs4500:0.1 .
	cp *.h ./part1
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; cmake ."
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make basic_example && ./basic_example"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make personal_test_suite && ./personal_test_suite"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_1 && ./submitted_test_1"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_2 && ./submitted_test_2"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_3 && ./submitted_test_3"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_4 && ./submitted_test_4"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_5 && ./submitted_test_5"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_6 && ./submitted_test_6"
	- docker run -ti -v "$$(pwd)/part1":/part1 cs4500:0.1 bash -c "cd ./part1; make submitted_test_7 && ./submitted_test_7"

second:
	docker build -t cs4500:0.1 . 
	cp *.h ./part2
	- docker run -ti -v "$$(pwd)/part2":/part2 cs4500:0.1 bash -c "cd ./part2; cmake ."
	- docker run -ti -v "$$(pwd)/part2":/part2 cs4500:0.1 bash -c "cd ./part2; make parallel_map_examples && ./parallel_map_examples"
	- docker run -ti -v "$$(pwd)/part2":/part2 cs4500:0.1 bash -c "cd ./part2; make parallel_map_personal_test_suite && ./parallel_map_personal_test_suite"

dircheck: ./Dockerfile ./Makefile ./object.h ./string.h ./helper.h ./part1 ./part1/README.md ./part1/dataframe.h ./part1/basic_example.cpp \
	./part1/submitted_test_1.cpp ./part1/submitted_test_2.cpp ./part1/submitted_test_3.cpp ./part1/submitted_test_4.cpp \
	./part1/submitted_test_5.cpp ./part1/submitted_test_6.cpp ./part1/submitted_test_7.cpp ./part1/personal_test_suite.cpp \
	./part2 ./part2/PARALLELMAPREADME.md ./part2/modified_dataframe.h ./part2/parallel_map_examples.cpp ./part2/parallel_map_personal_test_suite.cpp 
	echo "Directory structure looks OK"
