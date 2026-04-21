import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors

def extract_goal(kb):
    return tuple([int(i) for i in kb[0].split("(")[1].split(")")[0].split(",")])

def print_knowledge(kb):
    for i in range(1, len(kb), 2):
        if i + 1 < len(kb):
            print(kb[i], kb[i + 1])
        else:
            print(kb[i])

open_cells = {
    'Open(0,0)' : True, 'Open(0,1)' : False, 'Open(0,2)' : True, 'Open(0,3)' : True,  'Open(0,4)' : True,  'Open(0,5)' : True,
    'Open(1,0)' : True, 'Open(1,1)' : True,  'Open(1,2)' : True, 'Open(1,3)' : True,  'Open(1,4)' : True,  'Open(1,5)' : True,
    'Open(2,0)' : True, 'Open(2,1)' : False, 'Open(2,2)' : True, 'Open(2,3)' : False, 'Open(2,4)' : True,  'Open(2,5)' : True,
    'Open(3,0)' : True, 'Open(3,1)' : False, 'Open(3,2)' : True, 'Open(3,3)' : True,  'Open(3,4)' : False, 'Open(3,5)' : True,
    'Open(4,0)' : True, 'Open(4,1)' : True,  'Open(4,2)' : True, 'Open(4,3)' : True,  'Open(4,4)' : False, 'Open(4,5)' : True
}

visited_cells = {
    'Visited(0,0)' : True, 'Visited(0,1)' : False, 'Visited(0,2)' : False, 'Visited(0,3)' : False, 'Visited(0,4)' : False, 'Visited(0,5)' : False,
    'Visited(1,0)' : False, 'Visited(1,1)' : False, 'Visited(1,2)' : False, 'Visited(1,3)' : False, 'Visited(1,4)' : False, 'Visited(1,5)' : False,
    'Visited(2,0)' : False, 'Visited(2,1)' : False, 'Visited(2,2)' : False, 'Visited(2,3)' : False, 'Visited(2,4)' : False, 'Visited(2,5)' : False,
    'Visited(3,0)' : False, 'Visited(3,1)' : False, 'Visited(3,2)' : False, 'Visited(3,3)' : False, 'Visited(3,4)' : False, 'Visited(3,5)' : False,
    'Visited(4,0)' : False, 'Visited(4,1)' : False, 'Visited(4,2)' : False, 'Visited(4,3)' : False, 'Visited(4,4)' : False, 'Visited(4,5)' : False
}

# Rules for movement in different directions
# down right up left
"""movement_rules = [
    "At(x,y) & Open(x+1,y) & ~Visited(x+1,y) => Move(x,y,x+1,y)",
    "At(x,y) & Open(x,y+1) & ~Visited(x,y+1) => Move(x,y,x,y+1)",
    "At(x,y) & Open(x-1,y) & ~Visited(x-1,y) => Move(x,y,x-1,y)",
    "At(x,y) & Open(x,y-1) & ~Visited(x,y-1) => Move(x,y,x,y-1)"
]"""

# right down left up
movement_rules = [
    "At(x,y) & Open(x,y+1) & ~Visited(x,y+1) => Move(x,y,x,y+1)",
    "At(x,y) & Open(x-1,y) & ~Visited(x-1,y) => Move(x,y,x-1,y)",
    "At(x,y) & Open(x,y-1) & ~Visited(x,y-1) => Move(x,y,x,y-1)",
    "At(x,y) & Open(x+1,y) & ~Visited(x+1,y) => Move(x,y,x+1,y)"
]

# up right down left
"""movement_rules = [
    "At(x,y) & Open(x-1,y) & ~Visited(x-1,y) => Move(x,y,x-1,y)",
    "At(x,y) & Open(x,y+1) & ~Visited(x,y+1) => Move(x,y,x,y+1)",
    "At(x,y) & Open(x+1,y) & ~Visited(x+1,y) => Move(x,y,x+1,y)",
    "At(x,y) & Open(x,y-1) & ~Visited(x,y-1) => Move(x,y,x,y-1)"  
]"""

knowledge_base = list(["Goal(4,5)", "At(0,0)"])

def apply_rules(current_x, current_y, rules):
    temp_rules = list(rules)
    for i in range(0, len(temp_rules)):
        if '+' in temp_rules[i]:
            if temp_rules[i].split('+')[0][-1] == 'x':
                temp_rules[i] = temp_rules[i].replace('x+1', str(current_x + 1)).replace('x', str(current_x)).replace('y', str(current_y))
            elif temp_rules[i].split('+')[0][-1] == 'y':
                temp_rules[i] = temp_rules[i].replace('y+1', str(current_y + 1)).replace('x', str(current_x)).replace('y', str(current_y))
        elif '-' in temp_rules[i]:
            if temp_rules[i].split('-')[0][-1] == 'x':
                temp_rules[i] = temp_rules[i].replace('x-1', str(current_x - 1)).replace('x', str(current_x)).replace('y', str(current_y))
            elif temp_rules[i].split('-')[0][-1] == 'y':
                temp_rules[i] = temp_rules[i].replace('y-1', str(current_y - 1)).replace('x', str(current_x)).replace('y', str(current_y))
    return temp_rules


def chain_forward():
    backtrack_index = 1

    while True:
        moved = False
        current_x, current_y = tuple([int(i) for i in knowledge_base[-1].split("(")[1].split(")")[0].split(",")])
        next_position = ""
        temp_rules = apply_rules(current_x, current_y, movement_rules)

        for rule in temp_rules:
            next_cell = rule.split('=>')[1].strip().split('(')[1].split(')')[0].split(',')[2:4]
            nx = int(next_cell[0])
            ny = int(next_cell[1])
            if ((0 <= nx < 5) and (0 <= ny < 6)):
                next_position = f"At({nx},{ny})"
                
                antecedent, consequent = rule.split("=>")
                antecedent_facts = antecedent.strip().split("&")

                true_values = 0

                for fact in antecedent_facts:
                    fact = fact.strip()

                    if fact[0] == 'A' and fact in knowledge_base:
                        true_values += 1
                    elif fact[0] == 'O' and open_cells[fact]:
                        true_values += 1
                    elif fact[0] == '~' and not visited_cells[fact[1:]]:
                        visited_cells[fact[1:]] = True
                        true_values += 1
           
                if true_values == len(antecedent_facts):                    
                    knowledge_base.append(consequent.strip())
                    knowledge_base.append(next_position)
                    moved = True
                    backtrack_index = 1
                    break

        if not moved:
            i = -1
            move = 1
            previous_step = []
            while i != -(len(knowledge_base) + 1):
                if knowledge_base[i][0] == 'M':
                    if move == backtrack_index:
                        previous_step = knowledge_base[i].split("(")[1].split(")")[0].split(",")[0:2]
                        break
                    else:
                        move += 1
                i -= 1

            if not previous_step:
                print("No valid path from the starting point to the goal.")
                return
            
            knowledge_base.append(f"Backtracking Move({current_x},{current_y},{previous_step[0]},{previous_step[1]})")
            knowledge_base.append(f"At({previous_step[0]},{previous_step[1]})")
            backtrack_index += 1
            continue

        if visited_cells[f"Visited{str(extract_goal(knowledge_base)).replace(' ', '')}"]:
                print()
                print_knowledge(knowledge_base)
                print("Goal reached!")
                print()
                return

chain_forward()

# Visualization
maze = np.zeros((5, 6))  # 5 rows and 6 columns
visualization = np.zeros_like(maze)

# Coloring the visited cells
visited = [coords for coords, status in visited_cells.items() if status]
for coord in visited:
    x, y = map(int, coord.strip('Visited()').split(','))
    visualization[x, y] = 3  # 3 = visited cell (yellow)

# Coloring the open cells (obstacles)
obstacles = [coords for coords, status in open_cells.items() if not status]
for coord in obstacles:
    x, y = map(int, coord.strip('Open()').split(','))
    visualization[x, y] = 4  # 4 = obstacle (blue)

# Start and end cells (green)
start = (0, 0)
end = (4, 5)
visualization[start] = 2  # 2 = start/end cell (green)
visualization[end] = 2    # 2 = start/end cell (green)

# Creating the plot
fig, ax = plt.subplots(figsize=(5, 6))

# Define colormap
cmap = mcolors.ListedColormap(['white', 'gray', 'green', 'yellow', 'red', 'blue'])
bounds = [0, 1, 2, 3, 4, 5]  # Value categories
norm = mcolors.BoundaryNorm(bounds, cmap.N)

# Display maze
cax = ax.imshow(visualization, cmap=cmap, norm=norm)

# Title and axis labels
ax.set_title('Maze Visualization')
ax.set_xticks(np.arange(0, maze.shape[1], 1))
ax.set_yticks(np.arange(0, maze.shape[0], 1))
ax.set_xticklabels(np.arange(0, maze.shape[1]))
ax.set_yticklabels(np.arange(0, maze.shape[0]))

# Add grid lines
ax.set_xticks(np.arange(maze.shape[1] + 1) - 0.5, minor=True)
ax.set_yticks(np.arange(maze.shape[0] + 1) - 0.5, minor=True)
ax.grid(which='minor', color='black', linestyle='-', linewidth=1)

# Show the plot
plt.show()
