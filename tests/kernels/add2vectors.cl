__kernel void main(__global float* A, const __global float *B, const __global float *C)
{
  int i = get_global_id(0);
  A[i]= B[i] + C[i];
}
