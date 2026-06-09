D ?= 3
M ?= 100

all: main_seg main_interval stabbing_query storage

# --- Compilations ---
main_seg: main_seg.cpp
	g++ -g -std=c++20 main_seg.cpp -o main_seg

main_interval: main_interval.cpp
	g++ -g -std=c++20 main_interval.cpp -o main_interval

stabbing_query: stabbing_query.cpp
	g++ -g -std=c++20 stabbing_query.cpp -o stabbing_query

storage: storage.cpp
	g++ -g -std=c++20 storage.cpp -o storage


# --- Tree Visualization (Graphviz) ---
draw-seg: main_seg
	./main_seg
	dot -Tpdf segment_tree.txt > segmentTree.pdf
	xdg-open segmentTree.pdf &

draw-interval: main_interval
	./main_interval
	dot -Tpdf interval_tree.txt > intervalTree.pdf
	xdg-open intervalTree.pdf &


# --- Experiments & Plots: Stabbing Query ---
run-stabbing: stabbing_query
	./stabbing_query $(D) $(M) > output_stabbing_query.txt

plot-stabbing:
	@if [ -f output_stabbing_query.txt ]; then \
		python3 trace_stabbing.py; \
	else \
		echo "Error: output_stabbing_query.txt could not be found. First run 'make run-stabbing'"; \
	fi


# --- Experiments & Plots: Storage ---
run-storage: storage
	./storage $(D) $(M) > output_storage.txt

plot-storage:
	@if [ -f output_storage.txt ]; then \
		python3 trace_storage.py; \
	else \
		echo "Error: output_storage.txt could not be found. First run 'make run-storage'"; \
	fi


# --- Cleanning ---
clean:
	rm -f main_seg main_interval stabbing_query storage *.pdf *.txt


