## Testing & Validation
### `kinematics/kinematics.py`
- add_joint_axis ✅
- add_initial_end_effector_pose ✅
- forward (kinematics) ✅
- inverse (kinematics) ✅
- jacobian ✅

### `kinematics/matrixUtils.py`
- print_matrix ✅
- copy_matrix ✅
- convert_to_radians ✅
- identity ✅
- zero ✅
- transpose ✅
- trace ✅
- inverse (matrix inverse) ✅
- pseudo_inverse ✅
- get_rot_mat ✅
- get_pos_vec ✅
- create_trn_mat ✅
- trn_mat_inverse ✅
- adjoint ✅
- exp3 ✅
- exp6 ✅
- log3 ✅
- log6 ✅
- norm ✅
- get_angle ✅
- add_scalar ✅
- sub_scalar ✅
- mul_scalar ✅
- div_scalar ✅
- add_matrix ✅
- sub_matrix ✅
- mul_matrix ✅
- mul_vector ✅
- vec_to_so3 ✅
- so3_to_vec ✅
- se3_to_vec ✅
- vec_to_se3 ✅

### `motor-control/positions.cpp`, `motor-control/positions.h`
- All functions working on simulation + integrated IRL testing ✅
