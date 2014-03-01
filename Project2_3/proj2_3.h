#ifndef _PROJ2_1_H
#define _PROJ2_1_H
#define PI 3.14159265
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
 
void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);

//Timer for calculate FPS
unsigned FrameCount = 0;
void TimerFunction(int);
void IdleFunction(void);

//Create VBO and Shaders
void Cleanup(void);
void CreateVBO(void);
void DestroyVBO(void);
void CreateShaders(void);
void DestroyShaders(void);
void MouseClick(int, int, int, int);
void KeyboardFunc(unsigned char key, int x, int y);


#endif //_PROJ2_1_H