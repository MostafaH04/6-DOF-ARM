from matrixUtils import MatrixUtils
from kinematics import Kinematics
import math

kin = Kinematics(3)
mat = MatrixUtils()

kin.add_joint_axis([0, 0,  1,  4, 0,    0])
kin.add_joint_axis([0, 0,  0,  0, 1,    0])
kin.add_joint_axis([0, 0, -1, -6, 0, -0.1])

kin.add_initial_end_effector_pose([[-1, 0,  0, 0],
                                  [0, 1,  0, 6],
                                  [0, 0, -1, 2],
                                  [0, 0,  0, 1]])

joint_angles = [math.pi/2.0, 3, math.pi]
transform = kin.forward(joint_angles)
print(transform)