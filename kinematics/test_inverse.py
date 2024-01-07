from kinematics import Kinematics
from matrixUtils import MatrixUtils

kin = Kinematics(3)
mat_utils = MatrixUtils()

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
    [0, 0, -1, 1.6858],
    [0, 0, 0, 1]
]

jac = [[0] * 3 for _ in range(6)]
jac_t = [[0] * 3 for _ in range(6)]
AA_t = [[0] * 6 for _ in range(6)]
A_tA = [[0] * 3 for _ in range(3)]
pinv = [[0] * 6 for _ in range(3)]

joint_angles_0 = [1.0, 2.5, 3]

joint_angles = kin.inverse(desired_transform, jac, pinv, jac_t, AA_t, A_tA, joint_angles_0, 0.01, 0.001, 20)
mat_utils.print_matrix(joint_angles, 1, 3, "Joint angles")