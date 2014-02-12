#include "proj3.h"

using namespace std;

int CurrentWidth = 800,CurrentHeight = 600,WindowHandle = 0;

float marker_x, marker_y;


GLuint VertexShaderId;
GLuint FragmentShaderId;
GLuint ProgramId;
GLuint VaoId;
GLuint VboId;
GLuint VeoId;
GLuint ColorBufferId;

GLuint MarkerVaoId;
GLuint TrackVboId;
GLuint MarkerId;
vector<glm::vec2> track_points;
vector<glm::vec3> exits;



void AddToTrackVector(GLfloat x, GLfloat y);
void GenRandomDisplacement(GLfloat magnitude, GLfloat &side1, GLfloat &side2);
int GetSquarePos(GLfloat point_x, GLfloat point_y);
bool ifWallExists(int p1, int p2);
bool CheckNewPoint(glm::vec2 NewPoint,glm::vec2 OldPoint);
bool CheckExits(glm::vec2 NewPoint,glm::vec2 OldPoint);

GLuint GetLeftTopPointElementOfASquare(int pos);

GLfloat vertices[] = 
{
	//1 row
	-1.0f,1.0f,
	-0.5f,1.0f,
	0.0f,1.0f,
	0.5f,1.0f,
	1.0f,1.0f,
	//2 row
	-1.0f,0.5f,
	-0.5f,0.5f,
	0.0f,0.5f,
	0.5f,0.5f,
	1.0f,0.5f,
	//3 row
	-1.0f,0.0f,
	-0.5f,0.0f,
	0.0f,0.0f,
	0.5f,0.0f,
	1.0f,0.0f,
	//4 row
	-1.0f,-0.5f,
	-0.5f,-0.5f,
	0.0f,-0.5f,
	0.5f,-0.5f,
	1.0f,-0.5f,
	//5 row
	-1.0f,-1.0f,
	-0.5f,-1.0f,
	0.0f,-1.0f,
	0.5f,-1.0f,
	1.0f,-1.0f,
};

GLuint element_buffer[]=
{
	//row1
	0,1,
	1,2,
	2,3,
	3,4,
	//row5
	20,21,
	21,22,
	22,23,
	23,24,
	//col1
	0,5,
	5,10,
	10,15,
	15,20,
	//col5
	4,9,
	9,14,
	14,19,
	19,24,
	//row2
	//5,6,
	//6,7,
	7,8,
	8,9,
	//row3
	//10,11,
	//11,12,
	12,13,
	//13,14,
	//row4
	15,16,
	//16,17,
	//17,18,
	18,19,
	//col2
	1,6,
	6,11,
	//11,16,
	//16,21,
	//col3
	//2,7,
	7,12,
	//12,17,
	//17,22,
	//col4
	//3,8,
	//8,13,
	//13,18,
	//18,23,
};
bool CheckExits(glm::vec2 NewPoint,glm::vec2 OldPoint)
{
	for(int i = 0; i != 2 ; ++i)
	{
		int pos =  exits[i].x;
		if(GetSquarePos(OldPoint.x, OldPoint.y) == pos)
		{
			if(pos == 0)
			{
				if( abs(  exits[i].y -  exits[i].z )  == 1)
				{
					if(NewPoint.y >= 0.5f)
					{
						return true;
					}

				}else
				{
					if(NewPoint.x<= -0.5f)
					{
						return true;
					}
				}

			}else if(pos == 1 || pos == 2)
			{
				if(NewPoint.y >= 0.5f)
				{
					return true;
				}

			}else if(pos == 3)
			{
				if( abs(  exits[i].y -  exits[i].z )  == 1)
				{
					if(NewPoint.y >= 0.5f)
					{
						return true;
					}

				}else
				{
					if(NewPoint.x >= 0.5f)
					{
						return true;
					}
				}
			}else if( pos == 4 || pos == 8)
			{
				if(NewPoint.x <= -0.5f)
				{
					return true;
				}
			}else if( pos == 7 || pos == 11)
			{
				if(NewPoint.x >= 0.5f)
				{
					return true;
				}

			}else if( pos == 12)
			{
				if( abs(  exits[i].y -  exits[i].z )  == 1)
				{
					if(NewPoint.y <= -0.5f)
					{
						return true;
					}

				}else
				{
					if(NewPoint.x<= -0.5f)
					{
						return true;
					}
				}

			}else if( pos == 13 || pos == 14){

				if(NewPoint.x<= -0.5f)
				{
					return true;
				}

			}else if(pos ==15)
			{
				if( abs(  exits[i].y -  exits[i].z )  == 1)
				{
					if(NewPoint.y <= -0.5f)
					{
						return true;
					}

				}else
				{
					if(NewPoint.x>= 0.5f)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

GLuint GetLeftTopPointElementOfASquare(int pos)
{
	if(pos<4)
	{
		return pos;
	}else if(pos < 8)
	{
		return pos+1;
	}else if(pos<12)
	{
		return pos+2;
	}else{	
		return pos+3;
	}
}

bool CheckNewPoint(glm::vec2 NewPoint,glm::vec2 OldPoint)
{
	//need to check the wall and exits here
	//Check the exits


	//Check the boundries
	if(GetSquarePos(NewPoint.x, NewPoint.y) == -1)
	{
		return false;
	}
	//-------------------------------------------------

	int newSquarePos = GetSquarePos(NewPoint.x, NewPoint.y);
	int oldSquarePos = GetSquarePos(OldPoint.x, OldPoint.y);
	if(newSquarePos == oldSquarePos +1 )	// if new square at the right side
	{
		int oldLTPoint = GetLeftTopPointElementOfASquare(oldSquarePos);
		return !ifWallExists(oldLTPoint+1,oldLTPoint+1+5);

	}else if(newSquarePos == oldSquarePos - 1)	// if new square at the left side
	{
		int oldLTPoint = GetLeftTopPointElementOfASquare(oldSquarePos);
		return !ifWallExists(oldLTPoint,oldLTPoint+5);

	}else if(newSquarePos == oldSquarePos + 4)	// if new square at the down side
	{
		int oldLTPoint = GetLeftTopPointElementOfASquare(oldSquarePos);
		return !ifWallExists(oldLTPoint+5+1,oldLTPoint+5);

	}else if(newSquarePos == oldSquarePos - 4)	// if new square at the up side
	{
		int oldLTPoint = GetLeftTopPointElementOfASquare(oldSquarePos);
		return !ifWallExists(oldLTPoint,oldLTPoint+1);
	}
}

bool ifWallExists(int p1, int p2)
{
	for(int i = 0; i != sizeof(element_buffer)/sizeof(GLuint); i += 2)
	{
		if((p1 == element_buffer[i] && p2 == element_buffer[i+1])  || (p2 == element_buffer[i] && p1 == element_buffer[i+1]))
		{
			return true;
		}
	}
	return false;
}


int GetSquarePos(GLfloat point_x, GLfloat point_y)
{
	float x = point_x - (-0.5f);
	float y = 0.5f - point_y;
	if(x >= 1.0f || x<= 0.0f || y>=1.0f || y<=0.0f)
	{
		return -1;
	}
	int square_x = x/0.25f;
	int square_y = y/0.25f;
	return square_y*4+square_x;

}


void animate(int)
{
	GLfloat incre_x, incre_y;

	if(!bPause  && !bComplete)
	{
		GenRandomDisplacement(0.25f/50.0f,incre_x,incre_y);
		//cout<<" Incre_X : "<<incre_x<<" Incre_Y : "<<incre_y<<endl;;
		glm::vec2 p = *(--track_points.end());
		glm::vec2 new_points(p.x+incre_x,p.y+incre_y);

		if(CheckExits(new_points,p))
		{
			cout<<"Find the Exit"<<endl;
			bComplete =  true;
		}

		if(!CheckNewPoint(new_points, p))
		{
			cout<<"Hit the wall"<<endl;
			bComplete =  true;
		}
		//cout<<" Newp_X : "<<new_points.x<<" Newp_Y : "<<new_points.y<<endl;
		//int s_pos = GetSquarePos(new_points.x, new_points.y);
		//cout<<"Square Pos: "<<s_pos<<endl;
		track_points.push_back(new_points);
		glutPostRedisplay();
		glutTimerFunc(17, animate, 0);
	}
}

void AddToTrackVector(GLfloat x, GLfloat y)
{
	glm::vec2 vec(x,y);
	track_points.push_back(vec);
}

void GenRandomDisplacement(GLfloat magnitude, GLfloat &side1, GLfloat &side2)
{
	
	GLfloat angle = ((float)(rand()%360))/360.0f*2*PI;
	//cout<<"Angle:"<<angle<<endl;
	side1 = magnitude * cos(angle);
	side2 = magnitude * sin(angle);
}


void removeWalls()
{
	srand(time(NULL));
	int exit1 = rand()%16;
	int exit2 = rand()%16;
	while(exit1 == exit2)
	{
		exit2 = rand()%16;
	}

	int pos1 = 0;
	if(exit1 <4)
	{
		pos1 = exit1;
	}else if(exit1 < 8)
	{
		pos1 = 12 + (exit1-4);
	}else if(exit1 < 12)
	{
		pos1 = (exit1-8)*4;
	}else if(exit1 < 16)
	{
		pos1 = (exit1-12)*4+3;
	}
	exits.push_back(glm::vec3(pos1,element_buffer[exit1*2],element_buffer[exit1*2+1]));
	element_buffer[exit1*2] = 0;
	element_buffer[exit1*2+1] = 0;

	int pos2 = 0;
	if(exit2 <4)
	{
		pos2 = exit2;
	}else if(exit2 < 8)
	{
		pos2 = 12 + (exit2-4);
	}else if(exit2 < 12)
	{
		pos2 = (exit2-8)*4;
	}else if(exit2 < 16)
	{
		pos2 = (exit2-12)*4+3;
	}

	exits.push_back(glm::vec3(pos2,element_buffer[exit2*2],element_buffer[exit2*2+1] ));
	element_buffer[exit2*2] = 0;
	element_buffer[exit2*2+1] = 0;
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

	
	CreateShaders();
	CreateVBO();
	bPause = false;
	bComplete = false;
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

	WindowHandle = glutCreateWindow("Freeglut_glew_demo");

	if(WindowHandle < 1)
	{
		std::cout<<"Error: Could not create new rendering window!"<<std::endl;
		exit(EXIT_FAILURE);
	}

	//2 callback function
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);

	

	//2 callback function for timer
	//glutIdleFunc(IdleFunction);

	//The amount of milliseconds that should pass before the function is called
	//The function to call when the amount of milliseconds have passed
	//The value to pass to the function

	//glutTimerFunc(0, TimerFunction, 0);
	glutTimerFunc(1000, animate, 0);

	glutMouseFunc(MouseClick);

	glutKeyboardFunc(KeyboardFunc);
	glutCloseFunc(Cleanup);
}




void CreateShaders(void)
{
	ProgramId =  LoadShaders("vshader.txt","fshader.txt");
	glUseProgram(ProgramId);
}




void CreateVBO(void)
{
	

	const GLfloat Marker_Vertices[] = 
	{
		-0.5f,-0.5f,
		0.5f,-0.5f,
		0.5f,0.5f,
		-0.5f,0.5f
	};
	

	glGenVertexArrays(1,&VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(1,&VboId);
	glBindBuffer(GL_ARRAY_BUFFER,VboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	//glGenBuffers(1,&MarkerId);
	//glBindBuffer(GL_ARRAY_BUFFER,MarkerId);
	//glBufferData(GL_ARRAY_BUFFER,sizeof(Marker_Vertices),Marker_Vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1,&VeoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VeoId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(element_buffer),element_buffer,GL_STATIC_DRAW);

	//for marker
	glGenVertexArrays(1,&MarkerVaoId);

	glBindVertexArray(MarkerVaoId);
	glGenBuffers(1,&MarkerId);
	glBindBuffer(GL_ARRAY_BUFFER,MarkerId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Marker_Vertices),Marker_Vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)(0));
	glEnableVertexAttribArray(0);

	//for track
	glGenBuffers(1,&TrackVboId);


	GenRandomStartPosition(marker_x, marker_y);
	AddToTrackVector(marker_x,marker_y);

}


void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}


void DestroyShaders(void)
{

	glUseProgram(0);

	// glDetachShader(ProgramId, VertexShaderId);
	// glDetachShader(ProgramId, FragmentShaderId);

	// glDeleteShader(FragmentShaderId);
	// glDeleteShader(VertexShaderId);

	 glDeleteProgram(ProgramId);
}
	


void DestroyVBO(void)
{


	// glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);

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
	//glViewport(0, 0, CurrentWidth, CurrentHeight);
	//glm::mat4 = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(ProgramId,"proj"), 1, GL_TRUE, glm::value_ptr(glm::mat4(1.0f)));
	++FrameCount;

	//Clear with the default color and clear the depth buffer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw labyrinth
	glBindVertexArray(VaoId);
	
	GLint modelLoc = glGetUniformLocation(ProgramId,"model");
	glm::mat4 model= glm::scale(glm::mat4(1.0f),glm::vec3(0.5f));
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));

	glUniform3fv(glGetUniformLocation(ProgramId,"color"),1,glm::value_ptr(glm::vec3(0.0f,0.0f,0.0f)));
	glDrawElements(GL_LINES,sizeof(element_buffer)/sizeof(GL_UNSIGNED_INT),GL_UNSIGNED_INT,0);
	
	//Draw Maker
	//glBindBuffer(GL_ARRAY_BUFFER, MarkerId);
	glBindVertexArray(MarkerVaoId);
	//float x,y;
	//GenRandomStartPosition(x,y);
	//cout<<"X: "<<x<<" Y: "<<y<<endl;
//	glBindBuffer(GL_ARRAY_BUFFER,MarkerId);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f/50.0f,1.0f/50.0f,1.0f));
//	glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(marker_x,marker_y ,0.0f));
//	glm::mat4 model2 = trans*scale;
//	glUniformMatrix4fv(glGetUniformLocation(ProgramId,"model"),1,GL_FALSE,glm::value_ptr(model2));
//	glUniform3fv(glGetUniformLocation(ProgramId,"color"),1,glm::value_ptr(glm::vec3(1.0f,0.0f,0.0f)));
//	glDrawArrays(GL_LINE_LOOP,0,4);
//;

	//Draw Track
	glBindBuffer(GL_ARRAY_BUFFER,MarkerId);
	glBufferData(GL_ARRAY_BUFFER,track_points.size()*sizeof(glm::vec2),&track_points[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)(0));
	//glEnableVertexAttribArray(0);
	model= glm::scale(glm::mat4(1.0f),glm::vec3(1.0f));
	glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(ProgramId,"color"),1,glm::value_ptr(glm::vec3(0.0f,0.0f,1.0f)));
	glDrawArrays(GL_LINE_STRIP,0,track_points.size());



	glutSwapBuffers();
	glutPostRedisplay();
}


void TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = (char*)
		malloc(512 + strlen("Freeglut_glew_demo"));
 
		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			"Freeglut_glew_demo",
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
		);
 
		glutSetWindowTitle(TempString);
		free(TempString);
	}
     
	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}


void IdleFunction(void)	//????
{
	glutPostRedisplay();
}





void MouseClick(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		GLfloat wx = 2.0*x/CurrentWidth - 1.0;
		GLfloat wy = 1.0-2.0*y/CurrentHeight;
		cout<<"Square Pos: "<<GetSquarePos(wx,wy)<<endl;
	}
}

int main(int argc, char* argv[])
{
	removeWalls();
	cout<<"Exit Square: "<<exits[0].x<<" Exit 1: "<<exits[0].y<<" "<<exits[0].z<<endl;
	cout<<"Exit Square: "<<exits[1].x<< " Exit 2 : "<<exits[1].y<<" "<<exits[1].z<<endl;
	Initialize(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}




GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
 
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
	std::string Line = "";
	while(getline(VertexShaderStream, Line))
		VertexShaderCode += "\n" + Line;
	VertexShaderStream.close();
	}
 
 
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
	std::string Line = "";
	while(getline(FragmentShaderStream, Line))
		FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();
	}
 
	GLint Result = GL_FALSE;
	int InfoLogLength;
 
	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
 
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
 
	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);
 
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
 
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}

//For Maker
void GenRandomStartPosition(float &x,float&y)
{
	srand(time(NULL));
	int square_x = rand()%4;
	int square_y = rand()%4;
	float pos_x = (float)(rand()%2500)/10000.0f;
	float pos_y = (float)(rand()%2500)/10000.0f;

	x = ((float)square_x*square_width + pos_x - 1.0f)/2.0f;
	y = ((float)square_y*square_height + pos_y - 1.0f)/2.0f;
}


void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'q':
		exit(1);
		break;
	case 'r':	
		track_points.clear();
		GenRandomStartPosition(marker_x, marker_y);
		AddToTrackVector(marker_x,marker_y);
		bPause = false;
		bComplete = false;
		animate(0);
		break;
	case 'p':
		if(bPause == true)
		{
			bPause = false;
			animate(0);
		}else{
			bPause = true;
		}
		break;
	default:
		break;
	}
}

