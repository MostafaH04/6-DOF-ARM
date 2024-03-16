from kinematics import Kinematics
from matrixUtils import MatrixUtils
import math

DOF = 6
kin = Kinematics(DOF)
mat_utils = MatrixUtils()

kin.add_joint_axis([0, 0, 1, 4, 0, 0])
kin.add_joint_axis([0, 0, 0, 0, 1, 0])
kin.add_joint_axis([0, 0, -1, -6, 0, -0.1])
kin.add_joint_axis([0, 0, 1, 4, 0, 0])
kin.add_joint_axis([0, 0, 0, 0, 1, 0])
kin.add_joint_axis([0, 0, -1, -6, 0, -0.1])

kin.add_initial_end_effector_pose([[-1, 0, 0, 0],
                                    [0, 1, 0, 6],
                                    [0, 0, -1, 2],
                                    [0, 0, 0, 1]])

desired_transform = [
    [0, 1, 0, -5],
    [1, 0, 0, 4],
    [0, 0, -1, 1.69],
    [0, 0, 0, 1]
]

jac = [[0] * DOF for _ in range(6)]
jac_t = [[0] * DOF for _ in range(6)]
AA_t = [[0] * 6 for _ in range(6)]
A_tA = [[0] * DOF for _ in range(DOF)]
pinv = [[0] * 6 for _ in range(DOF)]

joint_angles_0 = [1, 2, 3, 4, 5, 6]

joint_angles = kin.inverse(desired_transform, jac, pinv, jac_t, AA_t, A_tA, joint_angles_0, 0.01, 0.01, 50)

for i in range(DOF):
  # Angles must be within [0, 2pi]
  joint_angles[i] = joint_angles[i] % (2 * math.pi)
  joint_angles[i] = round(joint_angles[i], 2)

mat_utils.print_matrix([joint_angles], 1, DOF, "Joint angles")

# Desired Output:
# Joint angles:
# [1.57, 3, 3.14]

# What we're seeing
# Joint angles:
# [2.6206607865346006, 3.997469320516016, 3.035563651648674]