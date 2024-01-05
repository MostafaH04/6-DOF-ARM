from typing import List
from matrixUtils import MatrixUtils
class Kinematics:
  def __init__(self, num_joints: int) -> None:
    self.num_joints = num_joints
    self.num_joints_declared = 0
    self.joint_screw_axes = [[0, 0, 0, 0, 0, 0] for i in range(6)]
    self.initial_end_effector_pose = [[0, 0, 0, 0] for i in range(4)]
    self.mat_utils = MatrixUtils()


  def add_joint_axis(self, w_x: float, w_y: float, w_z: float, v_x: float, v_y: float, v_z: float) -> None:
    self.joint_screw_axes[self.num_joints_declared][0] = w_x
    self.joint_screw_axes[self.num_joints_declared][1] = w_y
    self.joint_screw_axes[self.num_joints_declared][2] = w_z
    self.joint_screw_axes[self.num_joints_declared][3] = v_x
    self.joint_screw_axes[self.num_joints_declared][4] = v_y
    self.joint_screw_axes[self.num_joints_declared][5] = v_z

    self.num_joints_declared += 1

  def add_initial_end_effector_pose(self, initial_pose: List[List[float]]) -> None:
    self.initial_end_effector_pose[0][0] = initial_pose[0][0]
    self.initial_end_effector_pose[0][1] = initial_pose[0][1]
    self.initial_end_effector_pose[0][2] = initial_pose[0][2]
    self.initial_end_effector_pose[0][3] = initial_pose[0][3]

    self.initial_end_effector_pose[1][0] = initial_pose[1][0]
    self.initial_end_effector_pose[1][1] = initial_pose[1][1]
    self.initial_end_effector_pose[1][2] = initial_pose[1][2]
    self.initial_end_effector_pose[1][3] = initial_pose[1][3]

    self.initial_end_effector_pose[2][0] = initial_pose[2][0]
    self.initial_end_effector_pose[2][1] = initial_pose[2][1]
    self.initial_end_effector_pose[2][2] = initial_pose[2][2]
    self.initial_end_effector_pose[2][3] = initial_pose[2][3]

    self.initial_end_effector_pose[3][0] = initial_pose[3][0]
    self.initial_end_effector_pose[3][1] = initial_pose[3][1]
    self.initial_end_effector_pose[3][2] = initial_pose[3][2]
    self.initial_end_effector_pose[3][3] = initial_pose[3][3]

    def forward(self, joint_angles: List[List[float]]) -> List[List[float]]:
      vec6 = [0, 0, 0, 0, 0, 0]
      se3 = [[0, 0, 0, 0] for i in range(4)]
      exp6 = [[0, 0, 0, 0] for i in range(4)]
      result = [[0, 0, 0, 0] for i in range(4)]

      for i in range(4):
        for j in range(4):
          result[i][j] = self.initial_end_effector_pose[i][j]
      
      for i in range(self.num_joints - 1, 0, -1):
        # Matrix operations
        pass
      
      return result

    
    def inverse(self, ew, ev) -> List[List[float]]:
      transform_sb = [[0, 0, 0, 0] for i in range(4)]
      transform_sb_inv = [[0, 0, 0, 0] for i in range(4)]
      transform_sb_inv_t = [[0, 0, 0, 0] for i in range(4)]
      log6 = [[0, 0, 0, 0] for i in range(4)]
      vec6 = [0, 0, 0, 0, 0, 0]
      adj = [[0, 0, 0, 0, 0, 0] for i in range(6)]
      vs = [0, 0, 0, 0, 0, 0]
      w = [0, 0, 0]
      v = [0, 0, 0]
      pinv_vs = [0, 0, 0, 0, 0, 0]
      error = False
      i = 0

      # Matrix operations

      w[0] = vs[0]
      w[1] = vs[1]
      w[2] = vs[2]
      v[0] = vs[3]
      v[1] = vs[4]
      v[2] = vs[5]
      
      error = (self.mat_utils.norm(w) > ew) or (self.mat_utils.norm(v) > ev)

    def jacobian(self) -> List[List[float]]:
      pass