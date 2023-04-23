
board = [["5","3",".",".","7",".",".",".","."],
         ["6",".",".","1","9","5",".",".","."],
         [".","9","8",".",".",".",".","6","."],
         ["8",".",".",".","6",".",".",".","3"],
         ["4",".",".","8",".","3",".",".","1"],
         ["7",".",".",".","2",".",".",".","6"],
         [".","6",".",".",".",".","2","8","."],
         [".",".",".","4","1","9",".",".","5"],
         [".",".",".",".","8",".",".","7","9"]]

class Solution:
    def row_has_unique(self, board) -> bool:
        '''
        If all rows have unique numbers
        '''
        for r in range(9):
            # empty set to store already seen numbers
            s = set()
            for c in range(9):
                # if number already in set, break
                if (board[r][c] in s) and (board[r][c] != "."):
                    return False
                else:
                    s.add(board[r][c])

        # if not returned yet, implies elements in all rows were unique
        return True


    def col_has_unique(self, board) -> bool:
        '''
        If all columns have unique numbers
        '''

        # BELOW DOESNT WORK ON LISTS
        # dummy copy and populate the transpose
        # col_major_board = board
        # for r in range(9):
        #     for c in range(9):
        #         col_major_board[c][r] = board[r][c]

        # use list comprehension to find the transpose
        col_major_board = [[row[i] for row in board] for i in range(9)]

        print(col_major_board)
        # perform the same behaviour as row checks
        return self.row_has_unique(col_major_board)


    def box_has_unique(self, x, y, board) -> bool:
        '''
        If a box has unique numbers
        '''
        # empty set to store already seen numbers
        s = set()
        for r in range(x, x+3):
            for c in range(y, y+3):
                # if number already in set, break
                if (board[r][c] in s) and (board[r][c] != "."):
                    return False
                else:
                    s.add(board[r][c])
        return True


    def isValidSudoku(self, board: List[List[str]]) -> bool:
        unique_box = True
        for i in range(0, 9, 3):
            for j in range(0, 9, 3):
                if (self.box_has_unique(i, j, board) is False):
                    unique_box = False
                    break

        if (self.row_has_unique(board) and self.col_has_unique(board) and unique_box):
            return True
        else:
            return False