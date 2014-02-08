#ifndef _PROJ1_H
#define _PROJ1_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>


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

void removeWalls();
#endif //_PROJ1_H