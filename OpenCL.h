#ifndef _OPENCL_
#define _OPENCL_

#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)
const int length = 1000;

//OpenCL
// Load the kernel source code into the array source_str
FILE *fp;
char *source_str;
size_t source_size;
// Get platform and device information
cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;   
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
cl_program program;
size_t global_item_size = length; // Process the entire lists
size_t local_item_size = length; // Process one item at a time
cl_context context;
cl_command_queue command_queue;
cl_kernel kernel;
cl_mem a_mem_obj;
cl_mem b_mem_obj;
cl_mem c_mem_obj;
cl_mem d_mem_obj;
cl_mem e_mem_obj;
cl_mem f_mem_obj;

float *position_x1 = (float*)malloc(sizeof(float)*length);
float *velocity_y1 = (float*)malloc(sizeof(float)*length);
float *position_x2 = (float*)malloc(sizeof(float)*length);
float *velocity_y2 = (float*)malloc(sizeof(float)*length);
float *position_result1 = (float*)malloc(sizeof(float)*length);
float *position_result2 = (float*)malloc(sizeof(float)*length);

void loadFile()
{
	fp = fopen("Vector2SomaKernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );
}

void createBuffers()
{
	//Select devices to use in execution
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	// Create an OpenCL context
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
 
    // Create a command queue
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
 
    // Create memory buffers on the device for each vector 
    a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
            length * sizeof(float), NULL, &ret);
    b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
            length * sizeof(float), NULL, &ret);
    c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            length * sizeof(float), NULL, &ret);
	d_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            length * sizeof(float), NULL, &ret);
	e_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            length * sizeof(float), NULL, &ret);
	f_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            length * sizeof(float), NULL, &ret);
}

void createProgram()
{
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
}

void copyArrays()
{
	// Copy the lists to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            length * sizeof(float), position_x1, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            length * sizeof(float), velocity_y1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
            length * sizeof(float), position_x2, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, d_mem_obj, CL_TRUE, 0, 
            length * sizeof(float), velocity_y2, 0, NULL, NULL);
}

void createAndExecuteKernel()
{
	// Create the OpenCL kernel
    kernel = clCreateKernel(program, "Vector2SomaKernel", &ret);
 
    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&d_mem_obj);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&e_mem_obj);
	ret = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&f_mem_obj);


	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
            &global_item_size, &local_item_size, 0, NULL, NULL);
}

void readMemoryBuffer()
{
	// Read the memory buffer on the device to the local variable
    ret = clEnqueueReadBuffer(command_queue, e_mem_obj, CL_TRUE, 0, length * sizeof(float), position_result1, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, f_mem_obj, CL_TRUE, 0, length * sizeof(float), position_result2, 0, NULL, NULL);
}

void Cleanup()
{
	// Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(c_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(position_x1);
    free(velocity_y1);
    free(position_x2);
	free(velocity_y2);
	free(position_result1);
	free(position_result2);
}

#endif