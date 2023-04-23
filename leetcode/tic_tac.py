class Solution:

    def count_occ(self, val, board):
        '''
        Returns number of occurances of val on board
        '''
        occ = 0
        for i in range(3):
            for j in range(3):
                if (board[i][j] == val):
                    occ += 1
        return occ

    def check_diag(self, val, board):
        if (board[0][0] == val and board[1][1] == val and board[2][2] == val) or \
           (board[0][2] == val and board[1][1] == val and board[2][0] == val):
            return True
        return False

    def check_row(self, val, board):
        for i in range(3):
            if (board[i] == val+val+val):
                return True
        return False

    def check_col(self, val, board):
        for i in range(3):
            if (board[0][i] + board[1][i] + board[2][i] == val+val+val):
                return True
        return False

    def check_won(self, val, board):
        return self.check_row(val, board) or self.check_col(val, board) or self.check_diag(val, board)

    def validTicTacToe(self, board: List[str]) -> bool:
        diff = self.count_occ("X", board) - self.count_occ("O", board)

        # difference has to be zero or 1, everything else is untrue
        if diff != 0 and diff != 1:
            return False

        # both can't win
        if self.check_won("O", board) and self.check_won("X", board):
            return False

        # X will win first, since it started first, don't care about O yet
        if diff == 1:
            if self.check_won("X", board):
                return True

        if diff == 0:
            # at any point, when diff is 0, x might have already won, if true, then
            # this case is not allowed, immediately return False
            if self.check_won("X", board) is True:
                return False

            # O has won
            if self.check_won("O", board) is True:
                return True

        # nobody won
        if (self.check_won("O", board) is False) and (self.check_won("X", board) is False):
            return True
