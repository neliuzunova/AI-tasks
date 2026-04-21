import heapq  
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap  
from matplotlib.animation import FuncAnimation
import numpy as np

# Assignment 1: Problem Solving by State Space Search
# Group tasks:

# Task 1: Maze representation using a 2D list (walkable = 0, wall = 1)
maze = [
    [0, 1, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 1, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0],
    [0, 0, 0, 0, 1, 0]
]

start = (0, 0)  # Starting position
end = (4, 5)    # Goal position

# Task 3: Define the function to get neighbors of a node
def get_neighbors(node):
    neighbors = []
    x, y = node
    moves = [(0, -1), (1, 0), (0, 1), (1, 0)] 
    
    for dx, dy in moves:
        nx, ny = x + dx, y + dy
        if 0 <= nx < len(maze) and 0 <= ny < len(maze[0]) and maze[nx][ny] == 0:
            neighbors.append((nx, ny))
    return neighbors

# Task 4: Heuristic function (Manhattan distance)
def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

# Task 5: Path function to accumulate past nodes
def reconstruct_path(came_from, current):
    path = []
    while current in came_from:
        path.append(current)
        current = came_from[current]
    path.append(current)
    path.reverse()
    return path

# Task 6: Cost function to accumulate total cost of the path
def calculate_cost(g_score, path):
    total_cost = 0
    for node in path:
        total_cost += g_score.get(node, 0)
    return total_cost

# Task 7: A* algorithm for searching the path
def a_star(start, end):
    open_list = []
    closed_list = set()
    
    g_score = {start: 0}
    f_score = {start: heuristic(start, end)}
    
    heapq.heappush(open_list, (f_score[start], start))
    came_from = {}
    
    open_list_trace = []  
    
    while open_list:
        _, current = heapq.heappop(open_list)
        closed_list.add(current)
        
        # Task 2: Success criteria: if we reached the goal
        if current == end:
            path = reconstruct_path(came_from, current)
            return path, open_list_trace
        
        for neighbor in get_neighbors(current):
            if neighbor in closed_list:
                continue
            
            tentative_g_score = g_score[current] + 1 
            
            if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g_score
                f_score[neighbor] = g_score[neighbor] + heuristic(neighbor, end)
                
                heapq.heappush(open_list, (f_score[neighbor], neighbor))
                open_list_trace.append(neighbor)
    
    return None, open_list_trace

# Task 8: Output the optimal path
path, visited = a_star(start, end)
if path:
    print("Path found:", path)
else:
    path = []
    print("No path found")

# Visualize the search process
def visualize_maze_search(path, visited, start, end):
    fig, ax = plt.subplots()

    maze_copy = np.array(maze)
    
    cmap = ListedColormap(['white', 'red', 'blue', 'orange', 'green', 'yellow'])  # Custom colors
    maze_copy[start[0], start[1]] = 4  # Start in green
    maze_copy[end[0], end[1]] = 5    # End in yellow
    
    im = ax.imshow(maze_copy, cmap=cmap)

    def update(frame):
        if frame < len(visited):
            (x, y) = visited[frame]
            maze_copy[x, y] = 3  # Visited nodes in orange
        elif frame == len(visited):
            for (x, y) in path:  # Path in blue
                maze_copy[x, y] = 2
        im.set_data(maze_copy)
        return [im]

    ani = FuncAnimation(fig, update, frames=len(visited) + 1 + len(path), interval=1000, repeat=False)
    plt.show()

visualize_maze_search(path, visited, start, end)


