import kinematics
import math

kin = kinematics.Kinematics(3)
mat = kinematics.MatrixUtils()

kin.add_joint_axis([0, 0,  1,  4, 0,    0])
kin.add_joint_axis([0, 0,  0,  0, 1,    0])
kin.add_joint_axis([0, 0, -1, -6, 0, -0.1])

kin.add_initial_end_effector_pose([[-1, 0,  0, 0],
                                  [0, 1,  0, 6],
                                  [0, 0, -1, 2],
                                  [0, 0,  0, 1]])

joint_angles = [math.pi/2.0, 3, math.pi]
transform = kin.forward(joint_angles)

print(transform[0])
print(transform[1])
print(transform[2])
print(transform[3])

# Desired Output:
# [0.00    1.00    0.00    -5.00]
# [1.00    -0.00   0.00    4.00]
# [0.00    0.00    -1.00   1.69]
# [0.00    0.00    0.00    1.00]