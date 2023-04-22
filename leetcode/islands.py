import numpy as np
import queue


class ISLAND:
    def __init__(self, grid):
        self.grid = grid
        self.visited = np.zeros_like(grid)
        self.max_x, self.max_y = np.shape(grid)[0] - 1, np.shape(grid)[1] - 1
        self.num_islands = 0
        self.iter = 0

        # add all in a FIFO to process later
        self.q = queue.Queue()

    def dfs(self, i, j):
        '''
        Eat up all the connected islands, i.e. visited them all until recursion stops
        This function is called when number of islands are incremented, hence,
        make sure that all the connected islands are marked visited here.
        In short, the scope of this DFS function is to only mark islands as visited.

        Hybrid approach: Recursion is used during DFS and queues are used during BFS, however, here,
        a hybrid approach also worked out, thanks to the gracious / astounding recursion mechanisms.
        '''
        if (self.grid[i][j] == 1) and (self.visited[i][j] == 0):
            self.iter += 1
            print(f'Iter: {self.iter}, co-ord: {i}, {j}')
            self.visited[i][j] = 1

            # find neighbours
            up    = max(0, i-1), j
            down  = min(self.max_x, i+1), j
            left  = i, max(0, j-1)
            right = i, min(self.max_y, j+1)

            # add neighbours to queue to check them later
            self.q.put(up)
            self.q.put(down)
            self.q.put(left)
            self.q.put(right)

            # visit each neighbour in a DFS manner
            while self.q.empty() is False:
                i, j = self.q.get()
                self.dfs(i, j)

    def parse(self):
        for i in range(np.shape(self.grid)[0]):
            for j in range(np.shape(self.grid)[1]):
                # do not even visit nodes that are water
                if ((self.visited[i][j] == 0) and (self.grid[i][j] == 1)):

                    print('Columbus says: new island discovered!')
                    # reset iterations to study behaviour of recursion
                    self.iter = 0

                    # the loop captures an unvisited island and calls DFS to visit neighbours
                    self.dfs(i, j)

                    # lets increment for each unvisited island
                    # but this can only be done safely if all other connected islands are
                    # marked as visited. The responsibility of marking all the other neigbouring
                    # connected islands as visited, lies with the function above
                    self.num_islands = self.num_islands + 1

                    print('*****************\n')


        return self.num_islands

def main():

    # test grid
    grid = np.array([[1, 0, 1, 0, 1, 0],
                     [0, 1, 0, 1, 1, 1],
                     [0, 1, 1, 0, 1, 1],
                     [1, 1, 0, 1, 1, 1]])

    print(ISLAND(grid).parse())

if __name__ == "__main__":
    main()
