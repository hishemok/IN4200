import numpy as np

def read_file_from_graph(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Find number of nodes and edges
    N = 0
    edges = 0
    for line in lines:
        if line.startswith('# Nodes:'):
            parts = line.split()
            N = int(parts[2])
            edges = int(parts[4])
            break

    print(f"Number of nodes found: {N}")
    print(f"Number of edges found: {edges}")

    # Initialize hyperlink matrix and outgoings
    hyperlink_matrix = np.zeros((N, N))
    outgoings = np.zeros(N, dtype=int)

    # Count outgoings
    for line in lines:
        if not line.startswith('#'):
            from_node, to_node = map(int, line.strip().split())
            outgoings[from_node] += 1

    # Fill hyperlink matrix
    for line in lines:
        if not line.startswith('#'):
            from_node, to_node = map(int, line.strip().split())
            hyperlink_matrix[to_node][from_node] = 1.0 / outgoings[from_node]

    return N, hyperlink_matrix

def find_dangling_pages(N, hyperlink_matrix):
    dangling_indices = np.zeros(N, dtype=int)
    is_dangling = 0

    for j in range(N):
        column_sum = np.sum(hyperlink_matrix[:, j])
        if column_sum == 0.0 or hyperlink_matrix[j][j] == 1.0:
            dangling_indices[j] = 1
            is_dangling = 1
        else:
            dangling_indices[j] = 0

    return dangling_indices, is_dangling

def PageRank_iterations(N, hyperlink_matrix, d, epsilon):
    print("\n\nPageRank_iterations")

    # Initialize PageRank scores
    xprev = np.ones(N) / N
    xcurr = np.zeros(N)

    iterations = 0
    diff = 2.0

    while diff > epsilon and iterations < 10000:
        # Find indices of dangling webpages
        dangling_indices, is_dangling = find_dangling_pages(N, hyperlink_matrix)

        # Compute W_prev (sum of scores of dangling nodes)
        W_prev = 0.0
        if is_dangling:
            W_prev = np.sum(xprev[dangling_indices == 1])

        # Update xcurr
        for i in range(N):
            xcurr[i] = (1.0 - d) / N + d * W_prev / N
            for j in range(N):
                xcurr[i] += d * hyperlink_matrix[i][j] * xprev[j]

        # Compute the maximum difference between xcurr and xprev
        diff = np.max(np.abs(xcurr - xprev))

        # Update xprev for the next iteration
        xprev = xcurr.copy()

        iterations += 1

    print(f"Iterations: {iterations}")
    return xprev

def main():
    filename = "Oblig1/small_webpage_graph.txt"
    N, hyperlink_matrix = read_file_from_graph(filename)

    print("Number of nodes:", N)
    print("Hyperlink matrix:")
    print(hyperlink_matrix)

    # Initialize PageRank parameters
    d = 0.85  # Damping factor
    epsilon = 1e-6  # Convergence threshold

    # Compute PageRank scores
    scores = PageRank_iterations(N, hyperlink_matrix, d, epsilon)

    # Print the PageRank scores
    print("PageRank scores:")
    for i in range(N):
        print(f"Node {i}: {scores[i]}")

if __name__ == "__main__":
    main()