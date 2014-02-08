#include "proj1.h"

using namespace std;

int CurrentWidth = 800,CurrentHeight = 600,WindowHandle = 0;

GLuint VertexShaderId;
GLuint FragmentShaderId;
GLuint ProgramId;
GLuint VaoId;
GLuint VboId;
GLuint VeoId;
GLuint ColorBufferId;

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
	5,6,
	6,7,
	7,8,
	8,9,
	//row3
	10,11,
	11,12,
	12,13,
	13,14,
	//row4
	15,16,
	16,17,
	17,18,
	18,19,
	//col2
	1,6,
	6,11,
	11,16,
	16,21,
	//col3
	2,7,
	7,12,
	12,17,
	17,22,
	//col4
	3,8,
	8,13,
	13,18,
	18,23,
};

void removeWalls()
{
	srand(time(NULL));
	int exit1 = rand()%16;
	int exit2 = rand()%16;
	while(exit1 == exit2)
	{
		exit2 = rand()%16;
	}
	element_buffer[exit1*2] = 0;
	element_buffer[exit1*2+1] = 0;
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

	//set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
	glutIdleFunc(IdleFunction);

	//The amount of milliseconds that should pass before the function is called
	//The function to call when the amount of milliseconds have passed
	//The value to pass to the function
	glutTimerFunc(0, TimerFunction, 0);
	glutMouseFunc(MouseClick);
	glutCloseFunc(Cleanup);
}




void CreateShaders(void)
{
	ProgramId =  LoadShaders("vshader.txt","fshader.txt");
	glUseProgram(ProgramId);
}




void CreateVBO(void)
{

	GLfloat vertices2[]=
	{
		1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f
	};

	GLfloat vertices[] = 
	{
		//1 row
		-1.0f,-1.0f,
		-0.5f,-1.0f,
		0.0f,-1.0f,
		0.5f,-1.0f,
		1.0f,-1.0f,
		//2 row
		-1.0f,-0.5f,
		-0.5f,-0.5f,
		0.0f,-0.5f,
		0.5f,-0.5f,
		1.0f,-0.5f,
		//3 row
		-1.0f,0.0f,
		-0.5f,0.0f,
		0.0f,0.0f,
		0.5f,0.0f,
		1.0f,0.0f,
		//4 row
		-1.0f,0.5f,
		-0.5f,0.5f,
		0.0f,0.5f,
		0.5f,0.5f,
		1.0f,0.5f,
		//5 row
		-1.0f,1.0f,
		-0.5f,1.0f,
		0.0f,1.0f,
		0.5f,1.0f,
		1.0f,1.0f
	};

	

	glGenVertexArrays(1,&VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(1,&VboId);
	glBindBuffer(GL_ARRAY_BUFFER,VboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1,&VeoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VeoId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(element_buffer),element_buffer,GL_STATIC_DRAW);
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

	++FrameCount;

	//Clear with the default color and clear the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glDrawArrays(GL_LINES,0,2);
	glDrawElements(GL_LINES,sizeof(element_buffer)/sizeof(GL_UNSIGNED_INT),GL_UNSIGNED_INT,0);
	
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

}

int main(int argc, char* argv[])
{
	removeWalls();
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