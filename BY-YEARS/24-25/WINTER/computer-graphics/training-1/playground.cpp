// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

//tutorial2
#include <common/shader.hpp>

using namespace glm;

int main(){
		// Initialise GLFW
	glewExperimental = true; // Needed for core profile
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW

	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//fragment z tutotriala 2
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

		// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );


	// komentarz dlaczego są dwa zestawy punktow:
	// domyslam sie ze calosc skaluje sie w zaleznosci od tego na czym jest wyswietlana
	// dlatego tutaj chcialem w miare ladnie policzyc z 0.1 od dolu i gory ale u mnie to brzydko wygladalo 
	// stad ten drugi zestaw pkt ktory u mnie ladnie wyglada. :)

		// An array of 3 vectors which represents 3 vertices
	// static const GLfloat g_vertex_buffer_data_1[] = { // z tego co rozumiem tu mamy punkty
	// 	0.0f, 0.9f, 0.0f, //1
	// 	-0.3465f, 0.3f, 0.0f, //2
	// 	0.3465f,  0.3f, 0.0f, //3
	// 	0.0f, 0.3f, 0.0f, //4
	// 	-0.3465f, -0.3f, 0.0f, //5
	// 	0.3465f,  -0.3f, 0.0f, //6
	// 	0.0f,  -0.3f, 0.0f, //7
	// 	-0.3465f,  -0.9f, 0.0f,  //8
	// 	0.3465f,  -0.9f, 0.0f, //9
	// };
	
	static const GLfloat g_vertex_buffer_data_1[] = { // z tego co rozumiem tu mamy punkty
		0.0f, 0.825f, 0.0f, //1
		-0.3465f, 0.225f, 0.0f, //2
		0.3465f,  0.225f, 0.0f, //3
		0.0f, 0.225f, 0.0f, //4
		-0.3465f, -0.375f, 0.0f, //5
		0.3465f,  -0.375f, 0.0f, //6
		0.0f,  -0.375f, 0.0f, //7
		-0.3465f,  -0.975f, 0.0f,  //8
		0.3465f,  -0.975f, 0.0f, //9
	};



		// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_1), g_vertex_buffer_data_1, GL_STATIC_DRAW);



	//

	do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		// glClear( GL_COLOR_BUFFER_BIT );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(1.0f, 0.0f, 0.42f, 0.0f);
		
		// Use our shader
		glUseProgram(programID);
		// Draw triangle...
		
		// Draw nothing, see you in tutorial 2 !
		//i guess to oznacza ze tu mam dac rzeczy do rysowania

				// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 9); // 9 punktow to 3 trojkaty
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );
		
		
}