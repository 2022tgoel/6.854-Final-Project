#!/bin/bash

echo "Compiling"
g++ -o dinics_prog -std=c++17 dinics.cpp
g++ -o dinics_lct_prog -std=c++17 dinics_with_lct.cpp

echo "Running dinics's..."

total_dinics_time=0

for i in {1..2}
do
	./dinics_prog < test1_graph_input.txt > test1_graph_output.txt

	timing=$(tail -1 test1_graph_output.txt)
	pat='([0-9]+\.[0-9]+)'
	[[ "$timing" =~ $pat ]]
	time=${BASH_REMATCH[0]}
	total_dinics_time=$(python3 -c "print($total_dinics_time+$time)")
	echo "Execution $i: $time"
done

echo "Average timing: $(python3 -c "print($total_dinics_time/2)")"




echo "Running dinics's with LCT..."

total_dinics_lct_time=0

for i in {1..10}
do
	./dinics_lct_prog < test1_graph_input.txt > test1_graph_output.txt

	timing=$(tail -1 test1_graph_output.txt)
	pat='([0-9]+\.[0-9]+)'
	[[ "$timing" =~ $pat ]]
	time=${BASH_REMATCH[0]}
	total_dinics_lct_time=$(python3 -c "print($total_dinics_lct_time+$time)")
	echo "Execution $i: $time"
done

echo "Average timing: $(python3 -c "print($total_dinics_lct_time/10)")"

