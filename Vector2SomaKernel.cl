__kernel void Vector2SomaKernel(__global float *position_x1, __global float *velocity_y1, __global float *position_x2, __global float *velocity_y2, __global float *position_result1, __global float *position_result2)
{
// Vector element index
int i = get_global_id(0);
position_result1[i] = position_x1[i] + velocity_y1[i];
position_result2[i] = position_x2[i] + velocity_y2[i];
}