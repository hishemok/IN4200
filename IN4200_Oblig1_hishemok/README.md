## Instructions 

### Standard
- Compile:
    gcc main.c read_graph_from_file1.c read_graph_from_file2.c PageRank_iterations1.c  PageRank_iterations2.c  top_n_webpages.c  -lm -o ./main  

- Example:
    ./main small.txt large.txt 0.85 1e-8 10 


### Open MP parallelization 
- Compile: 
    gcc main_omp.c read_graph_from_file2.c PageRank_iterations2_omp.c  top_n_webpages_omp.c -lm -fopenmp -o ./main_omp 

- Example:
    ./main_omp large.txt 0.85 1e-8 10

