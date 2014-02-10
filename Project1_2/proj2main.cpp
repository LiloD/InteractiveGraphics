#include "proj2.h"

using namespace std;

int CurrentWidth = 800,CurrentHeight = 600,WindowHandle = 0;

GLuint VertexShaderId;
GLuint FragmentShaderId;
GLuint ProgramId;
GLuint VaoId;
GLuint VboId;
GLuint VeoId;
//GLuint ColorBufferId;

GLuint selectedElementBufferId;
vector<GLuint> selected_element;
int select_pos;
vector<int> selected_squares;
GLuint* selected_element_buffer = NULL;
int selected_element_buffer_size = 0;

GLuint* invisiElementBufferData = NULL;
GLuint invisiElementBufferId;
vector<int> invisiLines;


bool rightclick = false;;

int map[]=
{
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};



GLuint element_buffer[]=
{
	//square 1
	0,1,		//wall
	1,6,
	6,5,
	5,0,		//wall

	//square 2
	1,2,		//wall
	2,7,
	7,6,
	6,1,

	//square 3
	2,3,		//wall
	3,8,
	8,7,
	7,2,
	
	//square 4
	3,4,		//wall
	4,9,		//wall
	9,8,
	8,3,

	//square 5
	5,6,		
	6,11,		
	11,10,
	10,5,		//wall

	//square 6
	6,7,		
	7,12,		
	12,11,
	11,6,		

	//square 7
	7,8,		
	8,13,		
	13,12,
	12,7,	

	//square 8
	8,9,		
	9,14,		//wall
	14,13,
	13,8,		

	//square 9
	10,11,
	11,16,
	16,15,
	15,10,	//wall
	
	//square 10
	11,12,
	12,17,
	17,16,
	16,11,	//wall

	//square 11
	12,13,
	13,18,
	18,17,
	17,12,	

	//square 12
	13,14,
	14,19,	//wall
	19,18,
	18,13,

	//square 13
	15,16,
	16,21,	
	21,20,	//wall
	20,15,	//wall
	
	//square 14
	16,17,
	17,22,	
	22,21,	//wall
	21,16,	
	
	//square 15
	17,18,
	18,23,	
	23,22,	//wall
	22,17,

	//square 16
	18,19,	//wall
	19,24,	
	24,23,	//wall
	23,18	
};

GLuint wall_pos[] = 
{
	0,3,4,8,12,13,
	19,29,35,45,
	50,51,54,58,61,62
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

	int exit1_pos = wall_pos[exit1];
	int exit2_pos = wall_pos[exit2];

	element_buffer[exit1_pos*2] = 0;
	element_buffer[exit1_pos*2+1] = 0;
	
	element_buffer[exit2_pos*2] = 0;
	element_buffer[exit2_pos*2+1] = 0;
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
	glutKeyboardFunc(KeyboardFunc);
	glutCloseFunc(Cleanup);
}


void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'q':
		exit(1);
		break;
	default:
		break;
	}
}


void CreateShaders(void)
{
	ProgramId =  LoadShaders("vshader.txt","fshader.txt");
	glUseProgram(ProgramId);
}




void CreateVBO(void)
{

	GLfloat vertices2[] = 
	{
		-0.5f,0.0f,1.0f,
		-0.25f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.25f,0.0f,1.0f
	};

	GLuint element_buffer2[]=
	{
		0,1,1,2,2,3
	};

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

	

	glGenVertexArrays(1,&VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(1,&VboId);
	glBindBuffer(GL_ARRAY_BUFFER,VboId);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)(0));
	glEnableVertexAttribArray(0);

	glGenBuffers(1,&VeoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VeoId);
	int i = sizeof(element_buffer)/sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(element_buffer),element_buffer,GL_STATIC_DRAW);
	glUniform1i(glGetUniformLocation(ProgramId,"selected"),0);

	glGenBuffers(1,&selectedElementBufferId);
	glGenBuffers(1,&invisiElementBufferId);
}


void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}


void DestroyShaders(void)
{

	glUseProgram(0);

	//glDetachShader(ProgramId, VertexShaderId);
	//glDetachShader(ProgramId, FragmentShaderId);

	//glDeleteShader(FragmentShaderId);
	//glDeleteShader(VertexShaderId);

	 glDeleteProgram(ProgramId);
}
	


void DestroyVBO(void)
{


	// glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VeoId);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &selectedElementBufferId);
	glDeleteBuffers(1, &invisiElementBufferId);

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
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	//Clear with the default color and clear the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLineWidth(3.0f);

	glUniform1i(glGetUniformLocation(ProgramId,"selected"),0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VeoId);
	glDrawElements(GL_LINES,sizeof(element_buffer)/sizeof(GL_UNSIGNED_INT),GL_UNSIGNED_INT,0);
	
	glUniform1i(glGetUniformLocation(ProgramId,"selected"),2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,invisiElementBufferId);
	glDrawElements(GL_LINES,invisiLines.size(),GL_UNSIGNED_INT,0);

	if(rightclick == true)
	{
		glUniform1i(glGetUniformLocation(ProgramId,"selected"),1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,selectedElementBufferId);
		glDrawElements(GL_LINES,selected_element_buffer_size,GL_UNSIGNED_INT,0);
	}
	
	//glDrawElements(GL_LINES,6,GL_UNSIGNED_INT,0);
	
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
	//clicknum++;
	//Get the clicked square's position  ->pos
	GLfloat wx = 2.0*x/CurrentWidth - 1.0;
	GLfloat wy = 1.0-2.0*y/CurrentHeight;
	float row = 0.0f,col =-0.0f;
	int r = -1,c = -1;
	if(wy <= 0.5f && wy >= -0.5f&&wx <= 0.5f && wx >= -0.5f)
	{
		row = 0.5f - wy;
		r = row/0.25;

		col = -0.5f + wx;
		c = col/0.25 ;
		c+=3;
	}

	//select_pos = r*4+c;
	//int pos = select_pos;
	int pos =  r*4+c;

	if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
	{
		if(rightclick)
		{
			rightclick = false;
					
			selected_element.clear();
			selected_squares.clear();
			delete[] selected_element_buffer;			
			selected_element_buffer = NULL;
			return;

		}
		//----------------------------------------
		// handle the first mouse click
		if(!rightclick)
		{
			select_pos = r*4+c;
			//get all the squares adjesent to the sq we click on ->selected_squares
			//vector<int> selected_squares;
			selected_squares.push_back(pos);

			int up = pos - 4;
			if(up >= 0 )
			{
				selected_squares.push_back(up);
			}

			int down = pos+4;
			if(down<=15)
			{
				selected_squares.push_back(down);
			}

			if(pos != 3 && pos != 7 && pos != 11 && pos != 15)
			{
				int right = pos+1;
				selected_squares.push_back(right);
			}

			if(pos != 0 && pos != 4 && pos != 8 && pos != 12)
			{
				int left = pos - 1;
				selected_squares.push_back(left);
			}
			//-----------------------------------------------------------------------

			//Get all the squares' lines indices   --> selected_element_buffer
			for(vector<int>::iterator itr = selected_squares.begin(); itr != selected_squares.end(); ++itr)
			{
				for(int i = 0; i < 8; ++i)
				{
					selected_element.push_back(element_buffer[(*itr)*8+i]);
				}
			}

			selected_element_buffer_size = selected_element.size();
			selected_element_buffer = new GLuint[selected_element_buffer_size];

			for(int i = 0; i<selected_element_buffer_size; ++i)
			{
				selected_element_buffer[i] = selected_element[i];
			}
			//---------------------------------------------------------------------

			//glGenBuffers(1,&selectedElementBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,selectedElementBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT)*selected_element_buffer_size, selected_element_buffer,GL_STATIC_DRAW);
	
		
			selected_element.clear();

			delete[] selected_element_buffer;
			selected_element_buffer = NULL;
			rightclick = true;
		}
	}

	if (state == GLUT_DOWN&&button == GLUT_LEFT_BUTTON){
		//handle the second click
		if( rightclick)
		{
			if( pos != select_pos &&  ifInVectorInt(selected_squares, pos) )
			{
				if(pos == select_pos-4) // if click the up squre
				{
					invisiLines.push_back(element_buffer[select_pos*8]);
					invisiLines.push_back(element_buffer[select_pos*8+1]);

				}else if(pos == select_pos+4 )	//if click the down squre
				{
					invisiLines.push_back(element_buffer[select_pos*8+4]);
					invisiLines.push_back(element_buffer[select_pos*8+5]);

				}else if(pos == select_pos+1)	//if click the right squre
				{
					invisiLines.push_back(element_buffer[select_pos*8+2]);
					invisiLines.push_back(element_buffer[select_pos*8+3]);

				}else if(pos == select_pos-1)	//if click the left squre
				{
					invisiLines.push_back(element_buffer[select_pos*8+6]);
					invisiLines.push_back(element_buffer[select_pos*8+7]);
				}
			}else{
				//clicknum = 1;
				return;
			}

			//set up the inivisible lines element buffer
			//glGenBuffers(1,&invisiElementBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,invisiElementBufferId);
			//create the data
			invisiElementBufferData = new GLuint[invisiLines.size()];
			for( int i = 0; i !=invisiLines.size() ; ++i)
			{
				invisiElementBufferData[i] = invisiLines[i];
			}
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,invisiLines.size()*sizeof(GL_UNSIGNED_INT), invisiElementBufferData,GL_STATIC_DRAW);
			
			delete[] invisiElementBufferData;
			invisiElementBufferData = NULL;
			selected_squares.clear();
			//clicknum = 0;
			rightclick = false;
		}
	}		
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

bool ifInVectorInt(vector<int> v, int num)
{
	for(vector<int>::iterator itr = v.begin(); itr != v.end(); ++itr)
	{
		if( (*itr) == num)
		{
			return true;
		}
	}
	return false;
}