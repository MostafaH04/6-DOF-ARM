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
    return mat
  

  def zero(self, rows: int, cols: int) -> List[List[float]]:
    return [[0] * cols for _ in range(rows)]

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
    pivrow = 0
    pivrows = [0] * 6

    for k in range(n):
      tmp = 0
      for i in range(k, n):
        if abs(A[i][k]) >= tmp:
          tmp = abs(A[i][k])
          pivrow = i

      if A[pivrow][k] == 0.0:
        return None
        # or [[0]] ?

      if pivrow != k:
        for j in range(n):
          A[k][j], A[pivrow][j] = A[pivrow][j], A[k][j]
      
      pivrows[k] = pivrow

      tmp = 1.0 / A[k][k]
      A[k][k] = 1.0

      for j in range(n):
        A[k][j] *= tmp

      for i in range(n):
        if i != k:
          tmp = A[i][k]
          A[i][k] = 0.0
          for j in range(n):
            A[i][j] -= A[k][j] * tmp

    for k in range(n - 1, -1, -1):
      if pivrows[k] != k:
        for i in range(n):
          A[i][k], A[i][pivrows[k]] = A[i][pivrows[k]], A[i][k]

    return A


  def pseudo_inverse(self, mat: List[List[float]], A_t: List[List[float]], AA_t: List[List[float]], A_tA: List[List[float]], r: int, c: int) -> List[List[float]]:
    mat_t = self.transpose(mat, r, c, A_t)

    AA_t = self.mul_matrix(mat_t, A_t, r, c, c, r)
    A_tA = self.mul_matrix(A_t, mat_t, c, r, r, c)

    AA_t_inv = self.inverse(AA_t, r)
    A_tA_inv = self.inverse(A_tA, c)

    if AA_t_inv is not None:
      result = self.mul_matrix(A_t, AA_t, c, r, r, r)
    else:
      result = self.mul_matrix(A_tA, A_t, c, c, c, r)

    return result


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
  
  
  def trn_mat_inverse(self, mat: List[List[float]]) -> List[List[float]]:
    rot_mat = self.get_rot_mat(mat)
    pos_vec = self.get_pos_vec(mat)

    rot_mat_t = self.transpose(rot_mat, 3, 3)

    pos_vec_result = self.mul_vector(rot_mat_t, pos_vec, 3, 3)
    pos_vec_result = [-x for x in pos_vec_result]

    trn_mat_inv = self.create_trn_mat(rot_mat_t, pos_vec_result)
    return trn_mat_inv
  

  def adjoint(self, mat: List[List[float]]) -> List[List[float]]:
    result = self.zero(6, 6)
    rot_mat = self.get_rot_mat(mat)
    pos_vec = self.get_pos_vec(mat)
    so3 = self.vec_to_so3(pos_vec)

    bottom_left = self.mul_matrix(so3, rot_mat, 3, 3, 3, 3)

    for i in range(3):
      for j in range(3):
        result[i][j] = rot_mat[i][j]

    for i in range(3):
      for j in range(3):
        result[i+3][j] = bottom_left[i][j]

    for i in range(3):
      for j in range(3):
        result[i+3][j+3] = rot_mat[i][j]

    return result
  

  def exp3(self, mat: List[List[float]]) -> List[List[float]]:
    w = self.so3_to_vec(mat)

    if abs(self.norm(w)) < 1e-6:
      return self.identity(3)
    else:
      theta = self.get_angle(w)
      w_mat = self.div_scalar(mat, theta, 3, 3)
      w_mat_sq = self.mul_matrix(w_mat, w_mat, 3, 3, 3, 3)
      term2 = self.mul_scalar(w_mat, math.sin(theta), 3, 3)
      term3 = self.mul_scalar(w_mat_sq, 1 - math.cos(theta), 3, 3)
      result = self.add_matrix(self.identity(3), term2, 3, 3)
      result = self.add_matrix(result, term3, 3, 3)
      return result

  def exp6(self, mat: List[List[float]]) -> List[List[float]]:
    rot_mat = self.get_rot_mat(mat)
    pos_vec = self.get_pos_vec(mat)
    w = self.so3_to_vec(rot_mat)

    if abs(self.norm(w)) < 1e-6:
      return self.create_trn_mat(self.identity(3), pos_vec)
    else:
      theta = self.get_angle(w)
      w_mat = self.div_scalar(rot_mat, theta, 3, 3)
      exp3_rot = self.exp3(rot_mat)
      w_mat_sq = self.mul_matrix(w_mat, w_mat, 3, 3, 3, 3)

      result_pos_term1_a = self.mul_scalar(self.identity(3), theta, 3, 3)
      result_pos_term1_b = self.mul_scalar(w_mat, 1 - math.cos(theta), 3, 3)
      result_pos_term1_c = self.mul_scalar(w_mat_sq, theta - math.sin(theta), 3, 3)

      result_pos_term1 = self.add_matrix(result_pos_term1_a, result_pos_term1_b, 3, 3)
      result_pos_term1 = self.add_matrix(result_pos_term1, result_pos_term1_c, 3, 3)

      result_pos_term2 = self.div_scalar(pos_vec, theta, 1, 3)
      result_pos = self.mul_vector(result_pos_term1, result_pos_term2, 3, 3)

      return self.create_trn_mat(exp3_rot, result_pos)

  def log3(self, mat: List[List[float]]) -> List[List[float]]:
    acos_input = (self.trace(mat, 3) - 1.0) / 2.0

    if acos_input >= 1:
      return self.zero(3, 3)
    elif acos_input <= -1:
      w = [mat[2][1], mat[0][2], mat[1][0]]
      s = 1.0 / math.sqrt(2.0 * (1 + mat[2][2]))
      w = self.mul_scalar(w, s, 1, 3)
      w = self.mul_scalar(w, math.pi, 1, 3)
      return self.vec_to_so3(w)
    else:
      mat_t = self.transpose(mat, 3, 3)
      term = self.sub_matrix(mat, mat_t, 3, 3)
      theta = math.acos(acos_input)
      s = theta / 2.0 / math.sin(theta)
      return self.mul_scalar(term, s, 3, 3)

  def log6(self, mat: List[List[float]]) -> List[List[float]]:
    rot_mat = self.get_rot_mat(mat)
    w_mat = self.log3(rot_mat)
    condition = all(w == 0 for row in w_mat for w in row)

    if condition:
      rot = self.zero(3, 3)
      vec = [mat[0][3], mat[1][3], mat[2][3]]
      return self.create_trn_mat(rot, vec)
    else:
      theta = math.acos((self.trace(rot_mat, 3) - 1.0) / 2.0)
      w_mat_by_2 = self.div_scalar(w_mat, 2.0, 3, 3)
      w_mat_sq = self.mul_matrix(w_mat, w_mat, 3, 3, 3, 3)
      w_mat_sq_by_theta = self.div_scalar(w_mat_sq, theta, 3, 3)
      s = 1.0 / theta - 1.0 / math.tan(theta / 2.0) / 2.0

      term1 = self.sub_matrix(self.identity(3), w_mat_by_2, 3, 3)
      term2 = self.mul_scalar(w_mat_sq_by_theta, s, 3, 3)
      term3 = self.add_matrix(term1, term2, 3, 3)

      term4 = [mat[0][3], mat[1][3], mat[2][3]]
      p = self.mul_vector(term3, term4, 3, 3)

      return self.create_trn_mat(w_mat, p)


  def norm(self, vec: List[float]) -> float:
    return math.sqrt(sum(x**2 for x in vec))
  
  # ???
  def get_angle(self, vec: List[float]) -> float:
    return self.norm(vec)
  
  
  def add_scalar(self, vec: List[float], s: float) -> List[float]:
      return [x + s for x in vec]

  def sub_scalar(self, vec: List[float], s: float) -> List[float]:
      return [x - s for x in vec]

  def mul_scalar(self, vec: List[float], s: float) -> List[float]:
      return [x * s for x in vec]

  def div_scalar(self, vec: List[float], s: float) -> List[float]:
      return [x / s for x in vec]

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
    result = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
    result[0][0] = 0.0
    result[0][1] = -vec[2]
    result[0][2] = vec[1]

    result[1][0] = vec[2]
    result[1][1] = 0.0
    result[1][2] = -vec[0]

    result[2][0] = -vec[1]
    result[2][1] = vec[0]
    result[2][2] = 0.0

    return result

  def so3_to_vec(self, rot_mat: List[List[float]]) -> List[float]:
    result = [0.0] * 3
    result[0] = rot_mat[2][1]
    result[1] = rot_mat[0][2]
    result[2] = rot_mat[1][0]
    return result

  def se3_to_vec(self, trn_mat: List[List[float]]) ->  List[float]:
    result = [0.0] * 6
    result[0] = trn_mat[2][1]
    result[1] = trn_mat[0][2]
    result[2] = trn_mat[1][0]
    result[3] = trn_mat[0][3]
    result[4] = trn_mat[1][3]
    result[5] = trn_mat[2][3]
    return result

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

    return result