import numpy as np
from core import IKinBody
Blist = np.array([[0, 0, 1, 4, 0, 0],
                  [0, 0,  0, 0, 1,   0],
                  [0, 0, -1, -6, 0, -0.1]])
M = np.array([[-1, 0,  0, 0],
              [ 0, 1,  0, 6],
              [ 0, 0, -1, 2],
              [ 0, 0,  0, 1]])
T = np.array([[0, 1,  0,     -5],
              [1, 0,  0,      4],
              [0, 0, -1, 1.6858],
              [0, 0,  0,      1]])
thetalist0 = np.array([1.5, 2.5, 3])
eomg = 0.01
ev = 0.001

print(IKinBody(Blist, M, T, thetalist0, eomg, ev))