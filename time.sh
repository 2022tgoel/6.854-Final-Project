#!/bin/bash

echo "Compiling"
g++ -o dinics_prog -std=c++17 dinics.cpp
g++ -o dinics_lct_prog -std=c++17 dinics_with_lct.cpp
g++ -o mpm_prog -std=c++17 mpm.cpp

benchmark_algo () {
	algos=( "dinics" "dinics_lct" "mpm" )

	for algo in "${algos[@]}"
	do
		iters=3

		echo "Running ${algo}..."

		total_time=0
	 
		for (( i=1; i<=$iters; i++ ))
		do
			./${algo}_prog < graph_input.txt > graph_output.txt

			timing=$(tail -1 graph_output.txt)
			pat='([0-9]+\.[0-9]+)'
			[[ "$timing" =~ $pat ]]
			time=${BASH_REMATCH[0]}
			total_time=$(python3 -c "print($total_time+$time)")
			echo "Execution $i: $time"
		done

		echo "Average timing: $(python3 -c "print($total_time/$iters)")"
	done
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
