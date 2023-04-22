# Try out a quick Breadth first search
#  Level       Tree
#    0           0
#              /   \
#    1        1     2
#            / \    |
#    2      3   4   7
#           |   |
#    3      5   6
#              / \
#    4        8   9


import queue

class node:
    def __init__(self, num, adj=None):
        self.num = num
        self.adj = adj
        self.visited = False
        # As we traverse through a node, lets populate the level set
        self.level = 0

def build_graph():
    # initialize an empty graph and populate with nodes
    graph = {}
    graph[0] = node(0, [1, 2])
    graph[1] = node(1, [3, 4])
    graph[2] = node(2, [7])
    graph[3] = node(3, [5])
    graph[4] = node(4, [6])
    graph[5] = node(5, [])
    graph[6] = node(6, [8, 9])
    graph[7] = node(7, [])
    graph[8] = node(8, [])
    graph[9] = node(9, [])
    return graph

def bfs(graph, root_node):
    # initialize a FIFO queue
    q = queue.Queue()

    # level set to root node is zero
    graph[root_node].level = 0

    # add neighbours of the root node
    q.put(graph[root_node])

    # keep dequeing and queing until all nodes have been traversed
    while (q.empty() is False):

        # get current node and mark as visited
        n = q.get()
        n.visited = True
        print(f'{n.num}', end='->')
        # for each neighbour
        for neighbour in n.adj:
            # assign them a level set
            graph[neighbour].level = n.level + 1

            # add to queue for visiting later
            if graph[neighbour].visited == False:
                q.put(graph[neighbour])
    print('')

def main():
    g = build_graph()
    bfs(g, 0)

    # check if level sets are populated
    for key in g:
        n = g[key]
        print(n.num, n.visited, n.level)

if __name__ == "__main__":
    main()
