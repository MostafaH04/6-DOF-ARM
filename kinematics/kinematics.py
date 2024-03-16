from typing import List
from matrixUtils import MatrixUtils
class Kinematics:
  def __init__(self, num_joints: int) -> None:
    self.num_joints = num_joints
    self.num_joints_declared = 0
    self.joint_screw_axes = [[0] * 6 for _ in range(6)]
    self.initial_end_effector_pose = [[0] * 4 for _ in range(4)]
    self.mat_utils = MatrixUtils()


  def add_joint_axis(self, screw: List[float]) -> None:
    for i in range(6):
      self.joint_screw_axes[self.num_joints_declared][i] = screw[i]
    
    self.num_joints_declared += 1

  def add_initial_end_effector_pose(self, initial_pose: List[List[float]]) -> None:
    for i in range(4):
      for j in range(4):
        self.initial_end_effector_pose[i][j] = initial_pose[i][j]

  def forward(self, joint_angles: List[float]) -> List[List[float]]:
    vec6 = [0] * 6
    se3 = [[0] * 4 for _ in range(4)]
    exp6 = [[0] * 4 for _ in range(4)]
    result = [[0] * 4 for _ in range(4)]

    for i in range(4):
      for j in range(4):
        result[i][j] = self.initial_end_effector_pose[i][j]
    
    for i in range(self.num_joints-1, -1, -1):
      vec6 = self.mat_utils.mul_scalar(self.joint_screw_axes[i], joint_angles[i], 1, 6)
      se3 = self.mat_utils.vec_to_se3(vec6)
      exp6 = self.mat_utils.exp6(se3)
      result = self.mat_utils.mul_matrix(exp6, result, 4, 4, 4, 4)
    
    return result


  def inverse(self, transform: List[List[float]], jacobian: List[List[float]], pinv: List[List[float]], A_t: List[List[float]], AA_t: List[List[float]], A_tA: List[List[float]], initial_joint_angles: List[float], ew: float, ev: float, max_iterations: float) -> List[List[float]]:
    joint_angles = [0] * 6
    Tsb = [[0] * 4 for _ in range(4)]
    Tsb_inv = [[0] * 4 for _ in range(4)]
    Tsb_inv_T = [[0] * 4 for _ in range(4)]
    log6 = [[0] * 4 for _ in range(4)]
    vec6 = [0] * 6
    adj = [[0] * 6 for _ in range(6)]
    Vs = [0] * 6
    w = [0] * 3
    v = [0] * 3
    pinv_Vs = [0] * 6
    error = False
    i = 0

    joint_angles = initial_joint_angles
    Tsb = self.forward(joint_angles)
    Tsb_inv = self.mat_utils.trn_mat_inverse(Tsb)
    Tsb_inv_T = self.mat_utils.mul_matrix(Tsb_inv, transform, 4, 4, 4, 4)
    log6 = self.mat_utils.log6(Tsb_inv_T)
    vec6 = self.mat_utils.se3_to_vec(log6)
    adj = self.mat_utils.adjoint(Tsb)
    Vs = self.mat_utils.mul_vector(adj, vec6, 6, 6)

    for j in range(3):
      w[j] = Vs[j]
      v[j] = Vs[j + 3]
    
    error = (self.mat_utils.norm(w) > ew) or (self.mat_utils.norm(v) > ev)
    print("Error: ", error)

    while (error and i < max_iterations):
      i += 1

      print("========================================")
      print("Iteration:", i)
    
      jacobian = self.jacobian(joint_angles)
      print("Jacobian: ")
      print(jacobian[0])
      print(jacobian[1])
      print(jacobian[2])
      print(jacobian[3])
      print(jacobian[4])
      print(jacobian[5])

      pinv = self.mat_utils.pseudo_inverse(jacobian, 6, self.num_joints)
      print("Pseudo Inverse: ")
      print(pinv[0])
      print(pinv[1])
      print(pinv[2])

      pinv_Vs = self.mat_utils.mul_vector(pinv, Vs, self.num_joints, 6)
      print("Pseudo Inverse * Vs: ", pinv_Vs)

      joint_angles = self.mat_utils.add_matrix(joint_angles, pinv_Vs, 1, self.num_joints)
      print("Joint Angles: ")
      print(joint_angles)
      
      Tsb = self.forward(joint_angles)
      Tsb_inv = self.mat_utils.trn_mat_inverse(Tsb)
      Tsb_inv_T = self.mat_utils.mul_matrix(Tsb_inv, transform, 4, 4, 4, 4)
      log6 = self.mat_utils.log6(Tsb_inv_T)
      vec6 = self.mat_utils.se3_to_vec(log6)
      adj = self.mat_utils.adjoint(Tsb)
      Vs = self.mat_utils.mul_vector(adj, vec6, 6, 6)

      for j in range(3):
        w[j] = Vs[j]
        v[j] = Vs[j + 3]

      c1 = (self.mat_utils.norm(w) > ew)
      c2 = (self.mat_utils.norm(v) > ev)
      # error = (c1 or c2)

      is_pinv_Vs_zero = (pinv_Vs == [0.0] * 6)
      error = (c1 or c2) and not is_pinv_Vs_zero
      print()
    
    print("========================================")
    return joint_angles

  def jacobian(self, joint_angles: List[float]) -> List[List[float]]:
    transform = [[0] * 4 for _ in range(4)]
    vec6 = [0] * 6
    se3 = [[0] * 4 for _ in range(4)]
    exp6 = [[0] * 4 for _ in range(4)]
    adj = [[0] * 4 for _ in range(4)]
    jacobian_column = [0] * 6
    jacobian = [[0] * self.num_joints for _ in range(6)]

    transform = self.mat_utils.identity(4)

    for i in range(6):
      for j in range(self.num_joints):
        jacobian[i][j] = self.joint_screw_axes[j][i]
    
    for i in range(1, self.num_joints):
      vec6 = self.mat_utils.mul_scalar(self.joint_screw_axes[i-1], joint_angles[i-1], 1, 6)
      print("vec6: ", vec6)
      se3 = self.mat_utils.vec_to_se3(vec6)

      exp6 = self.mat_utils.exp6(se3)
      print("exp6: ")
      print(exp6[0])
      print(exp6[1])
      print(exp6[2])
      print(exp6[3])

      transform = self.mat_utils.mul_matrix(transform, exp6, 4, 4, 4, 4)
      print("transform: ")
      print(transform[0])
      print(transform[1])
      print(transform[2])
      print(transform[3])

      adj = self.mat_utils.adjoint(transform)
      jacobian_column = self.mat_utils.mul_vector(adj, self.joint_screw_axes[i], 6, 6)
      for j in range(6):
        jacobian[j][i] = jacobian_column[j]
    return jacobian