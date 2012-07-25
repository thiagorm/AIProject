#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <time.h>
#include <CL/cl.h>
#include "Agent.h"
#include "Vector2D.h"

#define MAX_SOURCE_SIZE (0x100000)
const int length = 1024;

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
size_t local_item_size = 64; // Process one item at a time
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
//Vector2D position;
//Vector2D velocity;
Agent agents[length];
Agent agent(Vector2D(0.0, 0.0), Vector2D(0.0, 0.0));
Agent target(Vector2D(100.0, 150.0), Vector2D(0.0, 0.0));
Vector2D steering;

// Tamanho inicial do quadrado
float x1 = 100.0f;
float y1 = 150.0f;
GLsizei rsize = 10;                                                    

// Tamanho do incremento nas dire��es x e y 
float xstep = 1.0f;
float ystep = 1.0f;

char texto[100];

// Largura e altura da janela
float windowWidth;
float windowHeight;

void DesenhaTexto(char *string) 
{  
  	glPushMatrix();
        // Posi��o que o texto ser� colocado          
        glRasterPos2f(-10,10-(10*0.08)); 
        // Exibe os caracteres
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++); 
	glPopMatrix();
}

//Fu��o que realiza o desenho
void Desenha(void)
{
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
                   

     glClear(GL_COLOR_BUFFER_BIT);

	 int j;
	 for(j=0;j<length;j++){
		 glColor3f(1.0f + j, 0.0f, 0.0f);
		 glBegin(GL_QUADS);
				   glVertex2f(agents[j].position.x,agents[j].position.y+rsize); 
				   glVertex2f(agents[j].position.x,agents[j].position.y);
				   glColor3f(1.0f + j, 0.0f, 0.0f);
				   glVertex2f(agents[j].position.x+rsize,agents[j].position.y);
				   glVertex2f(agents[j].position.x+rsize,agents[j].position.y+rsize);               
		 glEnd();
	 }

	 glColor3f(0.0f, 0.0f, 1.0f);

     glBegin(GL_QUADS);
               glVertex2f(target.position.x,target.position.y+rsize); 
               glVertex2f(target.position.x,target.position.y);
               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex2f(target.position.x+rsize,target.position.y);
               glVertex2f(target.position.x+rsize,target.position.y+rsize);               
     glEnd();

	 glColor3f(1.0f,1.0f,1.0f);
     DesenhaTexto(texto);
     glutSwapBuffers();
}

// Fun��o gameTime "Loop"
void Timer(int value)
{
    // Muda a dire��o quando chega na borda esquerda ou direita
    if(target.position.x > windowWidth-rsize || target.position.x < 0)
            xstep = -xstep;

    // Muda a dire��o quando chega na borda superior ou inferior
    if(target.position.y > windowHeight-rsize || target.position.y < 0)
          ystep = -ystep;
          
    
    //Move Agent
	/*steering = agent.calculate_steering(target.position);
	agent.position.x += steering.x;
	agent.position.y += steering.y;*/
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int a;
    for(a = 0; a < length; a++){
		position_x1[a] = agents[a].position.x;
		velocity_y1[a] = agents[a].calculate_steering(target.position).x;
		position_x2[a] = agents[a].position.y;
		velocity_y2[a] = agents[a].calculate_steering(target.position).y;
	}

	// Copy the lists to their respective memory buffers
    /*ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
            length * sizeof(float), position_x1, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
            length * sizeof(float), velocity_y1, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
            length * sizeof(float), position_x2, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, d_mem_obj, CL_TRUE, 0, 
            length * sizeof(float), velocity_y2, 0, NULL, NULL);

	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
 
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
 
    // Read the memory buffer on the device to the local variable
    ret = clEnqueueReadBuffer(command_queue, e_mem_obj, CL_TRUE, 0, length * sizeof(float), position_result1, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, f_mem_obj, CL_TRUE, 0, length * sizeof(float), position_result2, 0, NULL, NULL);

	printf("%f\n", position_result1[0]);
	printf("-----%f\n", position_result2[0]);*/

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int k;
	for(k = 0; k < length; k++){
		steering = agents[k].calculate_steering(target.position);
		agents[k].position.x += steering.x;
		agents[k].position.y += steering.y;
	}

	/*int k;
	for(k = 0; k < length; k++){
		agents[k].position.x = position_result1[k];
		agents[k].position.y = position_result2[k];
	}*/
	
	//printf(texto, "Position (%f,%f)", agent.position.x, agent.position.y);
	//printf("Position (%f,%f)\n", agent.position.x, agent.position.y);
    //printf("Position (%f,%f)\n", target.position.x, target.position.y);

    // Move target
    target.position.x -= xstep;
    target.position.y -= ystep;

    // Redesenha o quadrado com as novas coordenadas 
    glutPostRedisplay();
    glutTimerFunc(33,Timer, 1);
}

// Inicializa par�metros
void Inicializa (void)
{   
	int i;
	srand(time(NULL));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for( i = 0; i < length; i++)
		agents[i] = Agent(Vector2D((rand()%100), (rand()%100)), Vector2D(0.0, 0.0));

	fp = fopen("Vector2SomaKernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose( fp );

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

	int j;
	for(j = 0; j <= length; j++)
	{
		position_result1[j] = 0;
		position_result2[j] = 0;
	}
}

// Fun��o para realizar mudan�a no tamanho da janela 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     // Evita a divisao por zero
     if(h == 0) h = 1;
                           
     // Especifica as dimens�es da Viewport
     glViewport(0, 0, w, h);

     // Inicializa o sistema de coordenadas
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // Estabelece a janela de sele��o (left, right, bottom, top)     
     if (w <= h)  {
		windowHeight = 250.0f*h/w;
		windowWidth = 250.0f;
     }
     else  { 
		windowWidth = 250.0f*w/h;
		windowHeight = 250.0f;
     }
     
     gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

// Programa Principal 
int main(void)
{
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
     glutInitWindowSize(400,350);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Behaviors");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     glutTimerFunc(33, Timer, 1);
     Inicializa();
     glutMainLoop();
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