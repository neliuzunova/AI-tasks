from collections import deque
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import ListedColormap  
from matplotlib.animation import FuncAnimation

# Assignment 1: Problem Solving by State Space Search
# Individual work:

# Define the maze
maze = [
    [0, 1, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 1, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0],
    [0, 0, 0, 0, 1, 0]
]

start = (0, 0)
end = (4, 5)

# Define the function to get neighbors of a node
def get_neighbors(node):
    neighbors = []
    x, y = node
    moves = [(0, -1), (1, 0), (0, 1), (-1, 0)]
    
    for dx, dy in moves:
        nx, ny = x + dx, y + dy

        if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] == 0:
            neighbors.append((nx, ny))
    return neighbors

# Task 1: BFS Algorithm Implementation
def bfs(start, end):
    queue = deque([start])
    came_from = {start: None}
    # Task 2: Trace the attempted nodes during the search process
    visited = set([start])
    cost = {start: 0}
    visited_nodes = [start]
    
    while queue:
        current = queue.popleft()
        
        if current == end:
            path = []
            while current is not None:
                path.append(current)
                current = came_from[current]
            path.reverse()  # Reverse to get the path from start to end
            return path, visited_nodes, cost
        
        # Explore all the neighbors of the current node
        for neighbor in get_neighbors(current):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)
                came_from[neighbor] = current
                cost[neighbor] = cost[current] + 1 
                visited_nodes.append(neighbor)
    
    return None, visited_nodes, cost

# Run BFS to find the path and trace visited nodes
path, visited_nodes, cost = bfs(start, end)
if path:
    print("Path found:", path)
else:
    path = []
    print("No path found")

# Task 3: Function to visualize the search trace and path
def visualize_search_trace(path, visited_nodes, cost, start, end):
    fig, ax = plt.subplots()
    maze_copy = np.array(maze)
    
    cmap = ListedColormap(['white', 'red', 'blue', 'orange', 'green', 'yellow'])
    maze_copy[start[0], start[1]] = 4  # Green for start
    maze_copy[end[0], end[1]] = 5    # Yellow for end
    
    im = ax.imshow(maze_copy, cmap=cmap)

    def update(frame):
        if frame < len(visited_nodes):
            (x, y) = visited_nodes[frame]
            maze_copy[x, y] = 3  # Orange for visited nodes
            
            # Annotate the visited node with its cost
            ax.text(y, x, str(cost[(x, y)]), color='black', ha='center', va='center', fontsize=10)
            
        elif frame == len(visited_nodes):
            for (x, y) in path:
                maze_copy[x, y] = 2  # Blue for the path
                ax.text(y, x, str(cost[(x, y)]), color='black', ha='center', va='center', fontsize=10)
        
        im.set_data(maze_copy)
        return [im]

    ani = FuncAnimation(fig, update, frames=len(visited_nodes) + 1 + len(path), interval=200, repeat=False)

    plt.show()

visualize_search_trace(path, visited_nodes, cost, start, end)
