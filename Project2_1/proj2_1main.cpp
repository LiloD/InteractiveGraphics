#include "proj2_1.h"
#include "Program.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexBufferObject.h"
#include <ctime>
#include <cstdlib>

using namespace std;
void MotionFunc(int x, int y);

int CurrentWidth = 800,CurrentHeight = 600,WindowHandle = 0;

float marker_x, marker_y;

GLuint polygonVaoId;
GLuint edgeVaoId;

Program* shaderProgram;
Program* edgeShaderProgram;
Program* trackShaderProgram;

VertexBufferObject* vertexDataVbo;
VertexBufferObject* polygonIndicesVbo;
VertexBufferObject* edgeIndeicesVbo;

Camera camera;

bool leftClick = false;
int mouse_x, mouse_y;
float delta_x,delta_y;

//the fly motion track
GLuint trackVao;
GLuint trackVbo;
vector<glm::vec3> track_points;
vector<glm::vec3> face_normals;

bool bComplete = false;

//the exit face
GLuint exitVao;
GLuint exitVbo;
vector<glm::vec3> exit_face;

//Generate Random Vec3 Displacement for the fly
glm::vec3 GenRandomDisplacement();
void animate(int value);
void CreateNormals();
glm::vec3 GetVertexData(int i);
int CheckPoints(glm::vec3 point);
void GetExitFace(int index);
glm::vec3 GenRandomStart();


GLfloat vertices_data[] = 
{
	//---------------
	0.0f,1.0f,-0.5f,
	0.6f,0.6f,-0.5f,
	1.0f,0.0f,-0.5f,
	0.6f,-0.6f,-0.5f,
	0.0f,-1.0f,-0.5f,
	-0.6f,-0.6f,-0.5f,
	-1.0f,0.0f,-0.5f,
	-0.6f,0.6f,-0.5f,
	//---------------
	0.0f,1.0f,0.5f,
	0.6f,0.6f,0.5f,
	1.0f,0.0f,0.5f,
	0.6f,-0.6f,0.5f,
	0.0f,-1.0f,0.5f,
	-0.6f,-0.6f,0.5f,
	-1.0f,0.0f,0.5f,
	-0.6f,0.6f,0.5f
};

GLuint polygon_indices[] = 
{
	//back
	0,1,2,
	0,2,3,
	0,3,4,
	0,4,5,
	0,5,6,
	0,6,7,
	//front
	8,9,10,
	8,10,11,
	8,11,12,
	8,12,13,
	8,13,14,
	8,14,15,
	//---
	0,8,15,
	0,7,15,
	//---
	0,8,9,
	0,1,9,
	//---
	7,15,14,
	7,6,14,
	//---
	1,9,10,
	1,2,10,
	//---
	6,14,13,
	6,5,13,
	//---
	2,10,11,
	2,3,11,
	//----
	5,13,12,
	5,4,12,
	//-----
	3,11,12,
	3,4,12,
};

glm::vec2 faces[]=
{
	//back
	glm::vec2(0,5),
	//front
	glm::vec2(6,11),
	//1 clock
	glm::vec2(14,15),
	//2 clock
	glm::vec2(18,19),
	//4 clock
	glm::vec2(22,23),
	//5 clock
	glm::vec2(26,27),
	//7 clock
	glm::vec2(24,25),
	//8 clock
	glm::vec2(20,21),
	//10 clock
	glm::vec2(16,17),
	//11 clock
	glm::vec2(12,13),
};

GLuint edges_indices[] =
{
	//---
	0,1,
	1,2,
	2,3,
	3,4,
	4,5,
	5,6,
	6,7,
	7,0,
	//---
	8,9,
	9,10,
	10,11,
	11,12,
	12,13,
	13,14,
	14,15,
	15,8,
	//----
	0,8,
	1,9,
	2,10,
	3,11,
	4,12,
	5,13,
	6,14,
	7,15,
};

void GetExitFace(int index)
{
	exit_face.clear();

	int begin = faces[index].x*3;
	int end = begin+ 3*( faces[index].y -  faces[index].x+1 );

	for(int i = begin; i != end; ++i)
	{
		exit_face.push_back(
			glm::vec3( GetVertexData( polygon_indices[i] ) )
			);
	}
}

void CreateNormals()
{
	//back
	glm::vec3 normal1 =glm::cross(
		GetVertexData(1)-GetVertexData(0),
		GetVertexData(2)-GetVertexData(0)
		);
	face_normals.push_back(GetVertexData(0));
	face_normals.push_back(normal1);

	//front
	glm::vec3 normal2 =glm::cross(
		GetVertexData(10)-GetVertexData(8),
		GetVertexData(9)-GetVertexData(8)
		);
	face_normals.push_back(GetVertexData(8));
	face_normals.push_back(normal2);

	//1 clock
	glm::vec3 normal3 =glm::cross(
		GetVertexData(1)-GetVertexData(8),
		GetVertexData(0)-GetVertexData(8)
		
		);
	face_normals.push_back(GetVertexData(8));
	face_normals.push_back(normal3);

	//2 clock
	glm::vec3 normal4 =glm::cross(
		GetVertexData(2)-GetVertexData(9),
		GetVertexData(1)-GetVertexData(9)
		);
	face_normals.push_back(GetVertexData(9));
	face_normals.push_back(normal4);

	//4 clock
	glm::vec3 normal5 =glm::cross(
		GetVertexData(3)-GetVertexData(10),
		GetVertexData(2)-GetVertexData(10)
		);
	face_normals.push_back(GetVertexData(10));
	face_normals.push_back(normal5);

	//5 clock
	glm::vec3 normal6 =glm::cross(
		GetVertexData(4)-GetVertexData(11),
		GetVertexData(3)-GetVertexData(11)
	);
	face_normals.push_back(GetVertexData(11));
	face_normals.push_back(normal6);

	//7 clock
	glm::vec3 normal7 =glm::cross(
		GetVertexData(5)-GetVertexData(12),
		GetVertexData(4)-GetVertexData(12)
	);
	face_normals.push_back(GetVertexData(12));
	face_normals.push_back(normal7);

	//8 clock
	glm::vec3 normal8 =glm::cross(
		GetVertexData(6)-GetVertexData(13),
		GetVertexData(5)-GetVertexData(13)
		);
	face_normals.push_back(GetVertexData(13));
	face_normals.push_back(normal8);

	//10 clock
	glm::vec3 normal9 =glm::cross(
		GetVertexData(7)-GetVertexData(14),
		GetVertexData(6)-GetVertexData(14)
		);
	face_normals.push_back(GetVertexData(14));
	face_normals.push_back(normal9);

	//11 clock
	glm::vec3 normal10 =glm::cross(
		GetVertexData(8)-GetVertexData(15),
		GetVertexData(7)-GetVertexData(15)
		);
	face_normals.push_back(GetVertexData(15));
	face_normals.push_back(normal10);
}


int CheckPoints(glm::vec3 point)
{
	for(int i = 0; i != face_normals.size(); i+=2)
	{
		float rec = glm::dot(point-face_normals[i], face_normals[i+1]);
		if(rec >= 0)
		{
			cout<<"hit the wall"<<endl;
			return i/2;
		}
	}
	return -1;
}


glm::vec3 GetVertexData(int i)
{
	return glm::vec3(
		vertices_data[i*3],
		vertices_data[i*3+1],
		vertices_data[i*3+2]
		);
}

glm::vec3 GenRandomDisplacement()
{
	//float x = 1.0f - float(rand()%20000)/10000.0f;
	//float y = 1.0f - float(rand()%20000)/10000.0f;
	//float z = 1.0f - float(rand()%20000)/10000.0f;
	//return glm::normalize(glm::vec3(x,y,z));
	float verticalAngle =  ((float)(rand()%360))/360.0f*2*PI;
	float horizentalAngle =  ((float)(rand()%360))/360.0f*2*PI;
	return glm::vec3(cosf(verticalAngle)*sinf(horizentalAngle),sinf(horizentalAngle),cosf(verticalAngle)*sinf(horizentalAngle));
	
}



glm::vec3 GenRandomStart()
{
	srand(time(NULL));
	float x = 0.6f  - 1.2f*((float)(rand()%100000)/100000.0f);
	float y = 0.6f  - 1.2f*((float)(rand()%100000)/100000.0f);
	float z = 0.45f - 0.9f*((float)(rand()%100000)/100000.0f);
	return glm::vec3(x,y,z);
}

void animate(int value)
{
	if(!bComplete)
	{
		glm::vec3 old_point = *(--track_points.end());
		glm::vec3 displacement = GenRandomDisplacement();
		glm::vec3 new_point = old_point+ 1.0f/50.0f*displacement;
	
		int rec = CheckPoints(new_point);
		if(rec != -1)
		{
			bComplete = true;
			cout<<rec<<endl;

			GetExitFace(rec);
			for(int i = 0; i != exit_face.size(); ++i)
			{
				cout<<exit_face[i].x<<" "<<exit_face[i].y<<" "<<exit_face[i].z<<endl;
			}

		}

		track_points.push_back(new_point);
	
		glutPostRedisplay();
	
		glutTimerFunc(17, animate, 0);
	}


}

void Initialize(int argc, char* argv[])
{
	InitWindow(argc,argv);

	//Try to Init the Glew
	GLenum GlewInitResult;
	
	//The glewExperimental line is necessary to force GLEW to use a modern OpenGL method for checking if a function is available.
	glewExperimental = GL_TRUE;

	GlewInitResult = glewInit();
	if(GlewInitResult != GLEW_OK)
	{
		std::cout<<"Error: "<<glewGetErrorString(GlewInitResult)<<std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout<<"INFO: OpenGL Version: "<<glGetString(GL_VERSION)<<std::endl;
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	CreateShaders();
	CreateVBO();

	//-----------------------------------------------------------
	shaderProgram->use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view;
	view = camera.ViewMatrix();
	glm::mat4 proj;
	proj = glm::perspective(45.0f,800.0f/600.0f,0.1f,100.0f);

	shaderProgram->SetUniform("model", model);
	shaderProgram->SetUniform("proj", proj);
	shaderProgram->SetUniform("view", view);
	
	//--------------------------------------------------------
	edgeShaderProgram->use();
	model = glm::mat4(1.0f);
	//glm::mat4 view;
	view = camera.ViewMatrix();
	//glm::mat4 proj;
	proj = glm::perspective(45.0f,800.0f/600.0f,0.1f,100.0f);

	edgeShaderProgram->SetUniform("model", model);
	edgeShaderProgram->SetUniform("proj", proj);
	edgeShaderProgram->SetUniform("view", view);

	//----------------------------------------------------------
	trackShaderProgram->use();
	model = glm::mat4(1.0f);
	//glm::mat4 view;
	view = camera.ViewMatrix();
	//glm::mat4 proj;
	proj = glm::perspective(45.0f,800.0f/600.0f,0.1f,100.0f);

	trackShaderProgram->SetUniform("model", model);
	trackShaderProgram->SetUniform("proj", proj);
	trackShaderProgram->SetUniform("view", view);

	//set the clear color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}


void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);		// initialize the Freeglut library

	//Create OpenGL context
	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//return to the program when window is closed,otherwise Freeglut will end your program
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	

	//Create display Window
	glutInitWindowSize(CurrentWidth, CurrentHeight);
	glutInitWindowPosition(100, 100);

	//DEPTH refers to Z-Buffer
	//DOUBLE refers to back and front buffer
	//RGBA refers to color mode
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 

	WindowHandle = glutCreateWindow("CS537 Interactive Graphics Project 2_1 by Zhizhuo Ding");

	if(WindowHandle < 1)
	{
		std::cout<<"Error: Could not create new rendering window!"<<std::endl;
		exit(EXIT_FAILURE);
	}

	//2 callback function
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);

	glutWarpPointer(CurrentWidth/2, CurrentHeight/2);

	//2 callback function for timer
	glutIdleFunc(IdleFunction);

	//The amount of milliseconds that should pass before the function is called
	//The function to call when the amount of milliseconds have passed
	//The value to pass to the function
	//glutTimerFunc(0, TimerFunction, 0);
	
	glutTimerFunc(1000, animate, 0);
	
	glutMouseFunc(MouseClick);
	glutMotionFunc(MotionFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutCloseFunc(Cleanup);
	

	CreateNormals();
	for(int i = 0; i != face_normals.size();++i)
	{
		cout<<face_normals[i].x<<" "<<face_normals[i].y<<" "<< face_normals[i].z<<endl;
	}
}




void CreateShaders(void)
{
	//ProgramId =  LoadShaders("vshader.txt","fshader.txt");
	
	//glUseProgram(ProgramId);
	Shader vshader("shaders/vshader.txt",GL_VERTEX_SHADER);
	Shader fshader("shaders/fshader.txt",GL_FRAGMENT_SHADER);
	vector<GLuint> shaderIds;
	shaderIds.push_back(vshader.getId());
	shaderIds.push_back(fshader.getId());
	shaderProgram = new Program(shaderIds);
	shaderIds.clear();
	
	//shaderProgram->use();
	Shader vshader2("shaders/vshader2.txt",GL_VERTEX_SHADER);
	Shader fshader2("shaders/fshader2.txt",GL_FRAGMENT_SHADER);
	shaderIds.push_back(vshader2.getId());
	shaderIds.push_back(fshader2.getId());
	edgeShaderProgram = new Program(shaderIds);

	shaderIds.clear();
	Shader vshader3("shaders/vshader3.txt",GL_VERTEX_SHADER);
	Shader fshader3("shaders/fshader3.txt",GL_FRAGMENT_SHADER);
	shaderIds.push_back(vshader3.getId());
	shaderIds.push_back(fshader3.getId());
	trackShaderProgram = new Program(shaderIds);
}




void CreateVBO(void)
{
	

	vertexDataVbo = new VertexBufferObject();
	vertexDataVbo->CreateVBO();
	vertexDataVbo->bindVBO(GL_ARRAY_BUFFER);
	vertexDataVbo->addData(vertices_data,sizeof(vertices_data));
	vertexDataVbo->uploadDataToGPU(GL_STATIC_DRAW);
	
	polygonIndicesVbo = new VertexBufferObject();
	polygonIndicesVbo->CreateVBO();
	polygonIndicesVbo->bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	polygonIndicesVbo->addData(polygon_indices,sizeof(polygon_indices));
	polygonIndicesVbo->uploadDataToGPU(GL_STATIC_DRAW);

	
	edgeIndeicesVbo = new VertexBufferObject();
	edgeIndeicesVbo->CreateVBO();
	edgeIndeicesVbo->bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	edgeIndeicesVbo->addData(edges_indices,sizeof(edges_indices));
	edgeIndeicesVbo->uploadDataToGPU(GL_STATIC_DRAW);
	
	shaderProgram->use();
	glGenVertexArrays(1,&polygonVaoId);
	glBindVertexArray(polygonVaoId);
	vertexDataVbo->bindVBO(GL_ARRAY_BUFFER);
	polygonIndicesVbo->bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	GLuint attribLoc =  glGetAttribLocation(shaderProgram->getId(),"v_position");
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	edgeShaderProgram->use();
	glGenVertexArrays(1,&edgeVaoId);
	glBindVertexArray(edgeVaoId);
	vertexDataVbo->bindVBO(GL_ARRAY_BUFFER);
	edgeIndeicesVbo->bindVBO(GL_ELEMENT_ARRAY_BUFFER);
	attribLoc =  glGetAttribLocation(edgeShaderProgram->getId(),"v_position");
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc,3,GL_FLOAT,GL_FALSE,0,NULL);
	
	//Init track first start at (0,0,0)
	trackShaderProgram->use();
	//track_points.push_back(glm::vec3(0.0f,0.0f,0.0f));
	track_points.push_back(GenRandomStart());
	glGenBuffers(1,&trackVbo);
	glBindBuffer(GL_ARRAY_BUFFER,trackVbo);
	glBufferData(GL_ARRAY_BUFFER,track_points.size()*sizeof(glm::vec3),&track_points[0],GL_STATIC_DRAW);
	
	glGenVertexArrays(1,&trackVao);
	glBindVertexArray(trackVao);

	attribLoc =  glGetAttribLocation(edgeShaderProgram->getId(),"v_position");
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc,3,GL_FLOAT,GL_FALSE,0,NULL);

	//Init exit face
	glGenBuffers(1,&exitVbo);
	glBindBuffer(GL_ARRAY_BUFFER,exitVbo);

	glGenVertexArrays(1,&exitVao);
	glBindVertexArray(exitVao);

	attribLoc =  glGetAttribLocation(edgeShaderProgram->getId(),"v_position");
	glEnableVertexAttribArray(attribLoc);
	glVertexAttribPointer(attribLoc,3,GL_FLOAT,GL_FALSE,0,NULL);
}


void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}


void DestroyShaders(void)
{
	delete shaderProgram;
	delete edgeShaderProgram;
	delete trackShaderProgram;
}
	


void DestroyVBO(void)
{
	delete vertexDataVbo;
	delete polygonIndicesVbo;
	delete edgeIndeicesVbo;
}





void ResizeFunction(int w, int h)
{
	CurrentHeight = h;
	CurrentWidth = w;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

//where the drawing (rendering) of objects and the swapping of the back and front buffers occur.
void RenderFunction(void)
{	
	

	//Clear with the default color and clear the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shaderProgram->use();
	glm::mat4 view = camera.ViewMatrix();
	shaderProgram->SetUniform("view", view);

	glBindVertexArray(polygonVaoId);
	int i = sizeof(polygon_indices)/sizeof(GL_UNSIGNED_INT);
	glDrawElements(GL_TRIANGLES,i,GL_UNSIGNED_INT,0);

	edgeShaderProgram->use();
	view = camera.ViewMatrix();
	edgeShaderProgram->SetUniform("view", view);

	glBindVertexArray(edgeVaoId);
	i = sizeof(edges_indices)/sizeof(GL_UNSIGNED_INT);
	glDrawElements(GL_LINES,i,GL_UNSIGNED_INT,0);
	

	//draw track
	trackShaderProgram->use();
	view = camera.ViewMatrix();
	trackShaderProgram->SetUniform("view", view);



	glBindBuffer(GL_ARRAY_BUFFER,trackVbo);
	glBufferData(GL_ARRAY_BUFFER,track_points.size()*sizeof(glm::vec3),&track_points[0],GL_STATIC_DRAW);
	glBindVertexArray(trackVao);

	trackShaderProgram->SetUniform("flag", 1);
	glPointSize(10);
	glDrawArrays(GL_POINTS,0,1);

	trackShaderProgram->SetUniform("flag", 2);
	glDrawArrays(GL_LINE_STRIP,0,track_points.size());

	//if complete draw the exit face
	if(bComplete)
	{
		trackShaderProgram->SetUniform("flag", 1);
		glPointSize(10);
		glDrawArrays(GL_POINTS,track_points.size()-2,1);

		trackShaderProgram->SetUniform("flag", 0);
		glBindBuffer(GL_ARRAY_BUFFER,exitVbo);
		glBufferData(GL_ARRAY_BUFFER,exit_face.size()*sizeof(glm::vec3),&exit_face[0],GL_STATIC_DRAW);
		glBindVertexArray(exitVao);
		glDrawArrays(GL_TRIANGLES,0,exit_face.size());
	}

	glutSwapBuffers();
	glutPostRedisplay();
}




void IdleFunction(void)	
{
	if(leftClick)
	{
		camera.setHorizontalAngle(camera.getHorizontalAngle()+delta_x);
		camera.setVerticalAngle(camera.getVerticalAngle()+delta_y);

	
	}

	glutPostRedisplay();
}



void MotionFunc(int x, int y)
{
	if(leftClick)
	{

		delta_x  = float(mouse_x-x)*0.0005f;
		delta_y  = float(mouse_y-y)*0.0005f;
		mouse_x = x;
		mouse_y = y;
	}
	

	//cout<<leftClick<<endl;
	//cout<<"X :  " <<x<<"  Y:  "<<y<<endl;

	
}

 void MouseClick(int button, int state, int x, int y)
 {

 	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
 	{
		leftClick = true;
		mouse_y = y;
		mouse_x = x;
	}else{
		leftClick = false;
	}
 }

int main(int argc, char* argv[])
{
	Initialize(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}



 void KeyboardFunc(unsigned char key, int x, int y)
 {
 	switch(key)
 	{
 	case 'q':
 		exit(1);
 		break;
 	case 'w':
		camera.setPosition(camera.getPosition()+(camera.forward())*0.1f);
 		break;
 	case 'a':
		camera.setPosition(camera.getPosition()+(-camera.right())*0.1f);
 		break;
	case 's':
		camera.setPosition(camera.getPosition()+(-camera.forward())*0.1f);
 		break;
	case 'd':
		camera.setPosition(camera.getPosition()+(camera.right())*0.1f);
 		break;
 	default:
 		break;
 	}
 }



