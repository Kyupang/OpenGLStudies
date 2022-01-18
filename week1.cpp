#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


using namespace std;


int framebufferWidth, framebufferHeight;
GLuint triangleVertexArrayObject;
GLuint triangleShaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;


bool initShaderProgram() {

	//#3 shader�� �����. 
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"in vec3 positionAttribute;"
		"in vec3 colorAttribute;"
		"out vec3 passColorAttribute;" //glposition, vnormal, vtextcoord
		"void main()"
		"{"
		"gl_Position = vec4(positionAttribute, 1.0);" //< ��Ʈ������ ��ȯ�� ��ǥ�� 
		"passColorAttribute = colorAttribute;" //ī�� �׳� ��� 
		"}";


	//#4 
	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		"in vec3 passColorAttribute;"
		"out vec4 fragmentColor;"
		"void main()"
		"{"
		"fragmentColor = vec4(passColorAttribute, 1.0);" // pass
		"}";


	// shader object ���� program object�� �����Ұž� 
	// shader���� low-level�����͵�� rendering �� ����ϴ� �ֵ��̴�. program�� ���̴����� �ϳ��� �����ϰ� �̿� ���� ���̵������ �Ǿ���Ѵ�.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader); //������ ������� 

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		glDeleteShader(vertexShader);
		return false;
	}


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;

		return false;
	}




	//#5 �� shader object ��������! 
	triangleShaderProgramID = glCreateProgram();

	glAttachShader(triangleShaderProgramID, vertexShader);
	glAttachShader(triangleShaderProgramID, fragmentShader);

	glLinkProgram(triangleShaderProgramID);

	//�������� object ���� 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glGetProgramiv(triangleShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
		return false;
	}

	return true;
}



bool defineVertexArrayObject() {

	//#1 ��ǲ�����͵��� ��̿� �����Ѵ�. 
	//�ﰢ���� �����ϴ� vertex ������ - position�� color
	float position[] = {
		0.0f,  0.5f, 0.0f, //vertex 1  �� �߾�
		0.5f, -0.5f, 0.0f, //vertex 2  ������ �Ʒ�
		-0.5f, -0.5f, 0.0f //vertex 3  ���� �Ʒ�
	};

	float color[] = {
		1.0f, 0.0f, 0.0f, //vertex 1 : RED (1,0,0)
		0.0f, 1.0f, 0.0f, //vertex 2 : GREEN (0,1,0) 
		0.0f, 0.0f, 1.0f  //vertex 3 : BLUE (0,0,1)
	};



	//#2 ���۴� ���������� GPU�� �ٷ� �� �ִ� �ּ� �����̴�. cpu���� gpu�� ������ �� ����ϴ� interface
	//Vertex Buffer Object(VBO)�� �����Ͽ� vertex �����͸� �����Ѵ�.
	glGenBuffers(1, &trianglePositionVertexBufferObjectID); // buffer �� �����ϴ� �Լ� (����, �̸�) vertex�� index��ȯ
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID); //buffer �� binding ���� ���� ����(���� ��ü�� ������ ������ ����(target)�� �������־� ������ �޸𸮸� ���� ��ü�� �Ҵ��� �غ� �մϴ�.)
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW); //  ������ ���� ��ü�� ���� ���� �޸� ������ �Ҵ��ϰ� vertex �����͸�  ���� ��ü�� �����մϴ�. 

	glGenBuffers(1, &triangleColorVertexBufferObjectID);  //RGB buffer 
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);



	//#6 
	glGenVertexArrays(1, &triangleVertexArrayObject);
	glBindVertexArray(triangleVertexArrayObject);


	GLint positionAttribute = glGetAttribLocation(triangleShaderProgramID, "positionAttribute");
	if (positionAttribute == -1) {
		cerr << "position �Ӽ� ���� ����" << endl;
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(positionAttribute);

	GLint colorAttribute = glGetAttribLocation(triangleShaderProgramID, "colorAttribute");
	if (colorAttribute == -1) {
		cerr << "color �Ӽ� ���� ����" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);


	glBindVertexArray(0);


	return true;
}




void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//ó�� 2���� �Ķ���ʹ� viewport rectangle�� ���� �Ʒ� ��ǥ
	//���� 2���� �Ķ���ʹ� viewport�� �ʺ�� �����̴�.
	//framebuffer�� width�� height�� ������ glViewport���� ����Ѵ�.
	glViewport(0, 0, width, height);

	framebufferWidth = width;
	framebufferHeight = height;
}



void errorCallback(int errorCode, const char* errorDescription)
{
	cerr << "Error: " << errorDescription << endl;
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}




int main()
{

	glfwSetErrorCallback(errorCallback);


	if (!glfwInit()) {

		cerr << "Error: GLFW �ʱ�ȭ ����" << endl;
		std::exit(EXIT_FAILURE);
	}



	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	GLFWwindow* window = glfwCreateWindow(
		800,
		600,
		"KyuHwan",
		NULL, NULL);
	if (!window) {

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);



	glewExperimental = GL_TRUE;
	GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode) {

		cerr << "Error: GLEW �ʱ�ȭ ���� - " << glewGetErrorString(errorCode) << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}



	if (!GLEW_VERSION_3_3) {

		cerr << "Error: OpenGL 3.3 API is not available." << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}


	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;




	if (!initShaderProgram()) {

		cerr << "Error: Shader Program ���� ����" << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}



	if (!defineVertexArrayObject()) {

		cerr << "Error: Shader Program ���� ����" << endl;

		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}





	glfwSwapInterval(1);



	double lastTime = glfwGetTime();
	int numOfFrames = 0;
	int count = 0;





	glUseProgram(triangleShaderProgramID);
	glBindVertexArray(triangleVertexArrayObject);


	while (!glfwWindowShouldClose(window)) {


		double currentTime = glfwGetTime();
		numOfFrames++;
		if (currentTime - lastTime >= 1.0) {

			printf("%f ms/frame  %d fps \n", 1000.0 / double(numOfFrames), numOfFrames);
			numOfFrames = 0;
			lastTime = currentTime;
		}



		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 3);


		count++;

		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	glUseProgram(0);
	glBindVertexArray(0);


	glDeleteProgram(triangleShaderProgramID);
	glDeleteBuffers(1, &trianglePositionVertexBufferObjectID);
	glDeleteBuffers(1, &triangleColorVertexBufferObjectID);
	glDeleteVertexArrays(1, &triangleVertexArrayObject);
	glfwTerminate();

	std::exit(EXIT_SUCCESS);
}