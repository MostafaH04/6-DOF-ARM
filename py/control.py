import serial
import kinematics
import math

uart = serial.Serial(port="COM3", baudrate=115200, timeout=0.1)
DOF = 6

while True:
  kin = kinematics.Kinematics(DOF)
  mat_utils = kinematics.matrixUtils.MatrixUtils()

  kin.add_joint_axis([0,  0,  1,  4,  0,  0])
  kin.add_joint_axis([0,  0,  0,  0,  1,  0])
  kin.add_joint_axis([0,  0, -1,  -6, 0,  -0.1])
  kin.add_joint_axis([0, 0, 1, 4, 0, 0])
  kin.add_joint_axis([0, 0, 0, 0, 1, 0])
  kin.add_joint_axis([0, 0, -1, -6, 0, -0.1])

  kin.add_initial_end_effector_pose([[-1, 0, 0, 0],
                                      [0, 1, 0, 6],
                                      [0, 0, -1, 2],
                                      [0, 0, 0, 1]])

  jac = [[0] * DOF for _ in range(6)]
  jac_t = [[0] * DOF for _ in range(6)]
  AA_t = [[0] * 6 for _ in range(6)]
  A_tA = [[0] * DOF for _ in range(DOF)]
  pinv = [[0] * 6 for _ in range(DOF)]
  joint_angles_0 = [1, 2, 3, 4, 5, 6]
  transform = [[0] * 4 for _ in range(4)]

  for i in range(1, 5):
    while True:
      coords = input(f"Enter the desired transform (4x4 matrix - row {i}): ")
      input_coords = coords.split(" ")
      if len(input_coords) != 4:
        print("Invalid input. Please enter 4 coordinates.")
        continue
      for j in range(4):
        transform[i-1][j] = float(input_coords[j])
      break
  
  joint_angles = kin.inverse(transform, jac, pinv, jac_t, AA_t, A_tA, joint_angles_0, 0.01, 0.01, 50)
  mat_utils.print_matrix([joint_angles], 1, DOF, "Joint angles")

  output = ""
  for i in range(DOF):
    # Angles must be within [0, 2pi]
    joint_angles[i] = joint_angles[i] % (2 * math.pi)
    joint_angles[i] = round(joint_angles[i], 2)
    output += str(joint_angles[i]) + " "
  output = output[:-1] + "\r"

  print("jointAngles: ", output)
  uart.write(output.encode())