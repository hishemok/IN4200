# PageRank Program

## Instructions 

### 1. Standard Compilation
To compile the program using the standard approach:
gcc main.c read_graph_from_file1.c read_graph_from_file2.c PageRank_iterations1.c  PageRank_iterations2.c  top_n_webpages.c  -lm -o ./main  

Example Usage:
./main small.txt large.txt 0.85 1e-8 10 

### Arguments
 - small_file            - Input file with smaller graph data
 - large_file            - Input file with larger graph data
 - damping_factor        - Damping factor for the PageRank algorithm (ex: 0.85)
 - convergence_tolerance - Convergence tolerance for the algorithm (ex: 1e-8)
 - top_n_pages           - Number of top pages to display (ex: 10)

### Open MP Parallelization 
To compile the program with OpenMP parallelization:
gcc main_omp.c read_graph_from_file2.c PageRank_iterations2_omp.c  top_n_webpages_omp.c -lm -fopenmp -o ./main_omp 

Example Usage:
./main_omp large.txt 0.85 1e-8 10 

### Arguments
 - large_file            - Input file with larger graph data
 - damping_factor        - Damping factor for the PageRank algorithm (ex: 0.85)
 - convergence_tolerance - Convergence tolerance for the algorithm (ex: 1e-8)
 - top_n_pages           - Number of top pages to display (ex: 10)


## Notes
Both versions of the program calculate the ranking of web pages using the PageRank algorithm, which is commonly used to rank web pages based on their importance.

The OpenMP version can provide faster computations by utilizing multiple processor cores, therefore improving performance for large graphs.
