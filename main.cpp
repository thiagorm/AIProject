#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <time.h>
#include "Agent.h"
#include "Vector2D.h"
#include "OpenCL.h"

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

// Tamanho do incremento nas direções x e y 
float xstep = 1.0f;
float ystep = 1.0f;

char texto[100];

// Largura e altura da janela
float windowWidth;
float windowHeight;

void DesenhaTexto(char *string) 
{  
  	glPushMatrix();
        // Posição que o texto será colocado          
        glRasterPos2f(-10,10-(10*0.08)); 
        // Exibe os caracteres
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,*string++); 
	glPopMatrix();
}

//Fução que realiza o desenho
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

// Função gameTime "Loop"
void Timer(int value)
{
    // Muda a direção quando chega na borda esquerda ou direita
    if(target.position.x > windowWidth-rsize || target.position.x < 0)
            xstep = -xstep;

    // Muda a direção quando chega na borda superior ou inferior
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

	
	copyArrays();
	
 
    createAndExecuteKernel();
 
    readMemoryBuffer();

	//printf("%f\n", position_result1[0]);
	//printf("-----%f\n", position_result2[0]);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*int k;
	for(k = 0; k < length; k++){
		steering = agents[k].calculate_steering(target.position);
		agents[k].position.x += steering.x;
		agents[k].position.y += steering.y;
	}*/

	int k;
	for(k = 0; k < length; k++){
		agents[k].position.x = position_result1[k];
		agents[k].position.y = position_result2[k];
	}
	
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

// Inicializa parâmetros
void Inicializa (void)
{   
	int i;
	srand(time(NULL));

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	for( i = 0; i < length; i++)
		agents[i] = Agent(Vector2D((rand()%100), (rand()%100)), Vector2D(0.0, 0.0));

	loadFile();

	createBuffers();

	createProgram();

	int j;
	for(j = 0; j <= length; j++)
	{
		position_result1[j] = 0;
		position_result2[j] = 0;
	}
}

// Função para realizar mudança no tamanho da janela 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     // Evita a divisao por zero
     if(h == 0) h = 1;
                           
     // Especifica as dimensões da Viewport
     glViewport(0, 0, w, h);

     // Inicializa o sistema de coordenadas
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // Estabelece a janela de seleção (left, right, bottom, top)     
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
	 Cleanup();
}