#!/bin/bash

echo "Compiling"
g++ -o dinics_prog -std=c++17 dinics.cpp
g++ -o dinics_lct_prog -std=c++17 dinics_with_lct.cpp

benchmark_algo () {

	echo "Running dinics's..."

	total_dinics_time=0

	for i in {1..3}
	do
		./dinics_prog < graph_input.txt > graph_output.txt

		timing=$(tail -1 graph_output.txt)
		pat='([0-9]+\.[0-9]+)'
		[[ "$timing" =~ $pat ]]
		time=${BASH_REMATCH[0]}
		total_dinics_time=$(python3 -c "print($total_dinics_time+$time)")
		echo "Execution $i: $time"
	done

	echo "Average timing: $(python3 -c "print($total_dinics_time/2)")"


	echo "Running dinics's with LCT..."

	total_dinics_lct_time=0

	for i in {1..3}
	do
		./dinics_lct_prog < graph_input.txt > graph_output.txt

		timing=$(tail -1 graph_output.txt)
		pat='([0-9]+\.[0-9]+)'
		[[ "$timing" =~ $pat ]]
		time=${BASH_REMATCH[0]}
		total_dinics_lct_time=$(python3 -c "print($total_dinics_lct_time+$time)")
		echo "Execution $i: $time"
	done

	echo "Average timing: $(python3 -c "print($total_dinics_lct_time/10)")"

}

benchmark_file () {
	echo "Setting size = 30, sigma = 30"
	gsed -i 's/SIZE = [0-9]\+/SIZE = 30/g' image_to_graph.py
	gsed -i 's/SIGMA = [0-9]\+/SIGMA = 30/g' image_to_graph.py
	echo "Generating graph..."
	python3 image_to_graph.py

	benchmark_algo

	echo "Setting size = 75, sigma = 10"
	gsed -i 's/SIZE = [0-9]\+/SIZE = 75/g' image_to_graph.py
	gsed -i 's/SIGMA = [0-9]\+/SIGMA = 10/g' image_to_graph.py
	echo "Generating graph..."
	python3 image_to_graph.py

	benchmark_algo

	echo "Setting size = 200, sigma = 5"
	gsed -i 's/SIZE = [0-9]\+/SIZE = 200/g' image_to_graph.py
	gsed -i 's/SIGMA = [0-9]\+/SIGMA = 5/g' image_to_graph.py
	echo "Generating graph..."
	python3 image_to_graph.py

	benchmark_algo
}

echo "Setting name = test1"
gsed -i 's/^FILENAME = ".\+"$/FILENAME = "test1"/' image_to_graph.py

benchmark_file

echo "Setting name = test3"
gsed -i 's/^FILENAME = ".\+"$/FILENAME = "test3"/' image_to_graph.py

benchmark_file

echo "Setting name = testbaby"
gsed -i 's/^FILENAME = ".\+"$/FILENAME = "testbaby"/' image_to_graph.py

benchmark_file
