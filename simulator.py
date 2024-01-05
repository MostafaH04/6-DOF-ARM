import numpy as np
import pybullet as pb
from robot_descriptions.loaders.pybullet import load_robot_description

#Making an instance of a physical client
client = pb.connect(pb.GUI)

# Simulation parameters
pb.setGravity(0,0,-9.81)
pb.setRealTimeSimulation(0)

robot = None
try:
  robot = load_robot_description("ur10", useFixedBase=1)
except ModuleNotFoundError:
  robot = load_robot_description("ur10_description")

#Ending the program
input('Press any key to stop...')
pb.disconnect()