__kernel void main(__global float* A)
{
  A[get_global_id(0)]=10;
}
