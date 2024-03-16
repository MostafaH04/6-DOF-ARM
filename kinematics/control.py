import serial
import kinematics
import matrixUtils

uart = serial.Serial(port="COM3", baudrate=9600, timeout=0.1)

kin = kinematics.Kinematics(3)
mat_utils = matrixUtils.MatrixUtils()

kin.add_joint_axis([0,  0,  1,  4,  0,  0])
kin.add_joint_axis([0,  0,  0,  0,  1,  0])
kin.add_joint_axis([0,  0, -1,  -6, 0,  -0.1])

kin.add_initial_end_effector_pose([[-1, 0, 0, 0],
                                    [0, 1, 0, 6],
                                    [0, 0, -1, 2],
                                    [0, 0, 0, 1]])

jac = [[0] * 3 for _ in range(6)]
jac_t = [[0] * 3 for _ in range(6)]
AA_t = [[0] * 6 for _ in range(6)]
A_tA = [[0] * 3 for _ in range(3)]
pinv = [[0] * 6 for _ in range(3)]
joint_angles_0 = [1.0, 2.5, 3]
transform = [[0] * 4 for _ in range(4)]

while True:
  for i in range(1, 5):
    while True:
      coords = input(f"Enter the desired transform (4x4 matrix - row {i}): ")
      input_coords = coords.split(" ")  # array
      if len(input_coords) != 4:
        print("Invalid input. Please enter 4 coordinates.")
        continue
      for j in range(4):
        transform[i-1][j] = float(input_coords[j])
      break
  
  print("Desired transform: ", transform)
  joint_angles = kin.inverse(transform, jac, pinv, jac_t, AA_t, A_tA, joint_angles_0, 0.01, 0.01, 50)
  mat_utils.print_matrix([joint_angles], 1, 3, "Joint angles")

  uart.write(bytes(" ".join(str(joint_angles)), 'utf-8'))