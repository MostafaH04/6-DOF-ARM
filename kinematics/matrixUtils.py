from typing import List
import math

class MatrixUtils:
    def __init__(self) -> None:
        pass


    def print_matrix(self, mat: List[List[float]], r: int, c: int, message: str) -> None:
        print(message)
        for i in range(r):
            for j in range(c):
                print(mat[i][j], end = "\t")
            print()
        print()


    def copy_matrix(self, mat: List[List[float]], r: int, c: int) -> List[List[float]]:
        result = mat
        return result


    def identity(self, n: int) -> List[List[float]]:
        mat = [[0.0] * n for _ in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j:
                    mat[i][j] = 1
                else:
                    mat[i][j] = 0
    

    def zero(rows: int, cols: int) -> List[List[float]]:
        mat = [0.0] * (rows * cols)
        return mat


    def transpose(self, mat: List[List[float]], r: int, c: int) ->  List[List[float]]:
        result = [[0.0] * r for _ in range(c)]
        for i in range(r):
            for j in range(c):
                result[j][i] = mat[i][j]
        return result


    def trace(self, mat: List[List[float]], r: int) -> int:
        sum = 0
        for i in range(r):
            sum += mat[i][i]
        return sum
    

    def inverse(self, A: List[List[float]], n: int) -> List[List[float]]:
        pass


    def pseudo_inverse(self, mat: List[List[float]], A_t: List[List[float]], AA_t: List[List[float]], A_tA: List[List[float]], result: List[List[float]]) -> List[List[float]]:
        pass


    def get_rot_mat(self, mat: List[List[float]]) -> List[List[float]]:
        rot_mat = [row[:3] for row in mat[:3]]
        return rot_mat
    

    def get_pos_vec(self, mat: List[List[float]]) -> List[float]:
        pos_vec = [mat[i][3] for i in range(3)]
        return pos_vec
    

    def create_trn_mat(self, rot_mat: List[List[float]], pos_vec: List[List[float]]) -> List[List[float]]:
        trn_mat = self.zero(4, 4)

        for i in range(3):
            for j in range(3):
                trn_mat[i][j] = rot_mat[i][j]

        for i in range(3):
            trn_mat[i][3] = pos_vec[i]

        trn_mat[3][3] = 1.0

        return trn_mat
    
    
    def trm_mat_inverse(self, mat: List[List[float]]) -> List[List[float]]:
        rot_mat = self.get_rot_mat(mat)
        pos_vec = self.get_pos_vec(mat)

        rot_mat_t = self.transpose(rot_mat, 3, 3)

        pos_vec_result = self.mul_vector(rot_mat_t, pos_vec, 3, 3)
        pos_vec_result = [-x for x in pos_vec_result]

        trn_mat_inv = self.create_trn_mat(rot_mat_t, pos_vec_result)
        return trn_mat_inv
    

    def adjoint(self, mat: List[List[float]]) -> List[List[float]]:
        pass


    def norm(self, vec: List[float]) -> float:
        return math.sqrt(sum(x**2 for x in vec))
    
    # ???
    def get_angle(self, vec: List[float]) -> float:
        return self.norm(vec)
    
    
    def add_scalar(self, mat: List[List[float]], s: float, r: int, c: int) -> List[List[float]]:
        return [[mat[i][j] + s for j in range(c)] for i in range(r)]


    def sub_scalar(self, mat: List[List[float]], s: float, r: int, c: int) -> List[List[float]]:
        return [[mat[i][j] - s for j in range(c)] for i in range(r)]


    def mul_scalar(self, mat: List[List[float]], s: float, r: int, c: int) -> List[List[float]]:
        return [[mat[i][j] * s for j in range(c)] for i in range(r)]


    def div_scalar(self, mat: List[List[float]], s: float, r: int, c: int) -> List[List[float]]:
        return [[mat[i][j] / s for j in range(c)] for i in range(r)]


    def add_matrix(self, mat1: List[List[float]], mat2: List[List[float]], r: int, c: int) -> List[List[float]]:
        return [[mat1[i][j] + mat2[i][j] for j in range(c)] for i in range(r)]


    def sub_matrix(self, mat1: List[List[float]], mat2: List[List[float]], r: int, c: int) -> List[List[float]]:
        return [[mat1[i][j] - mat2[i][j] for j in range(c)] for i in range(r)]


    def mul_matrix(self, mat1: List[List[float]], mat2: List[List[float]], r1: int, c1: int, r2: int, c2: int) -> List[List[float]]:
        result = [[0.0] * c2 for i in range(r1)]
        for i in range(r1):
            for j in range(c2):
                result[i][j] = sum(mat1[i][k] * mat2[k][j] for k in range(c1))
        return result


    def mul_vector(self, mat: List[List[float]], vec: List[float], r: int, c: int) -> List[float]:
        result = [0.0] * c
        for i in range(r):
            for j in range(c):
                result[i] += vec[j] * mat[i][j]
        return result
    

    def vec_to_so3(self, vec: List[float]) -> List[List[float]]:
        result = self.zero(3, 3)
        result[0][0] = 0.0
        result[0][1] = -vec[2]
        result[0][2] = vec[1]

        result[1][0] = vec[2]
        result[1][1] = 0.0
        result[1][2] = -vec[0]

        result[2][0] = -vec[1]
        result[2][1] = vec[0]
        result[2][2] = 0.0

    def so3_to_vec(self, rot_mat: List[List[float]]) -> List[float]:
        result = [0.0] * 3
        result[0] = rot_mat[2][1]
        result[1] = rot_mat[0][2]
        result[2] = rot_mat[1][0]

    def se3_to_vec(self, trn_mat: List[List[float]]) ->  List[float]:
        result = [0.0] * 6
        result[0] = trn_mat[2][1]
        result[1] = trn_mat[0][2]
        result[2] = trn_mat[1][0]
        result[3] = trn_mat[0][3]
        result[4] = trn_mat[1][3]
        result[5] = trn_mat[2][3]

    def vec_to_se3(self, vec: List[float]) -> List[List[float]]:
        v = [vec[0], vec[1], vec[2]]
        so3_mat = self.vec_to_so3(v)

        result = self.zero(4, 4)

        for i in range(4):
            for j in range(4):
                result[i][j] = so3_mat[i][j]

        result[0][3] = vec[3]
        result[1][3] = vec[4]
        result[2][3] = vec[5]

        for i in range(3):
            result[3][i] = 0
            result[i][3] = 0

        result[3][0] = 0
        result[3][1] = 0
        result[3][2] = 0
        result[3][3] = 0

    
    