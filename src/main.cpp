#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 
     0.5f, -0.5f, 0.0f, 
     0.0f,  0.5f, 0.0f
};
float vertices2[] = {
    -0.8f, -0.8f, 0.0f, 
     0.2f, -0.8f, 0.0f, 
     -0.3f,  0.2f, 0.0f,
};

float rectangle_vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};
unsigned int indices[] = {
    0,1,3, // first triangle
    1,2,3  // second triangle
};

unsigned int vertexShader;
unsigned int fragmentShader, fragmentShader2;
int success;
char infoLog[512];

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    //  vertex shader 
    //  steps: 
    //  -create the shader with glCreateShader and the proper argument
    //  -attach the source code to the object (a C string)
    //  -compile shader and check status
    //   vertex shader turns 3D coordinates of our input vertices into Normalized 
    //   device coordinates
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL, infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    // fragment shader -> calculating the color output of the pixels 
    // colors in compiter graphics normaly use RGBA (red-green-blue-opacity)
    // steps are the same as vertex shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }

    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader2,512,NULL, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    // now that we have the shaders we need, in order to use them we need to 
    // link them to a shader program. It takes the output of each shader and gives it 
    // as input for the next one in the graphics pipeline. Therefore, we'll get 
    // linking errors if the inputs and outputs don't match for adjacent steps.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check linking status 

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"ERROR::LINKING_FAILED\n"<<infoLog<<std::endl;
    }
    //glUseProgram(shaderProgram);
    // now that we have linked the shaders to the program, we don't individually need 
    // them, so we might free that memory. 
    //glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int shaderProgram2;
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2,vertexShader);
    glAttachShader(shaderProgram2,fragmentShader2);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout<<"ERROR::LINKING_FAILED\n"<<infoLog<<std::endl;
    }

    // we can call OpenGL functions after GLAD init
    unsigned int VAO, VBO, EBO;
    glGenBuffers(1,&VBO);
    glGenBuffers (1, &EBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO); // VAO binded, saving attrib pointers  config and VBO, EBO
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // now, we need to tell openGL how to interpret our "vertices" data as 3 vertices 
    // with (x,y,z) coordinates instead of a plain 9 elements array.
    // the data in the vertices variable is tightly packed in the array and the first
    // value is at the beginning of the buffer.
    // with this knowledge we can tell OpenGL how it should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // VAO unbinded, bind another VAO to configure other object
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    // make sure to unbind VBO and EBO AFTER unbinding VAO, otherwise it stores the 
    // unbinding and ends up without the VBO, EBO.

    // the layout (location = 0) line in the vertex shader serves as an identifier 
    // for the shader parameter 'aPos'. That is the first argument, the shader parameter 
    // we want to configure. 
    // the second parameter of glVertexAttribPointer is the size of the shader's attribute
    // because 'aPos' is a vec3, we set it to 3. 
    // next we need to set the data type of the attribute. vec3 in GLSL is composed of 
    // floats, being GL_FLOAT the correct name.
    // the stride is the space in bytes between consecutive vertex attributes. 
    // here, it's:  4 bytes per coordinate (float = 4 bytes) * 3 coordinates = 12 for a 
    // (x,y,z) point.
    // the last parameter is the offset of where the position data begins in the buffer.
    // in this case, it's 0. 
    // -------we need to create the shader program after defining all the VAOs? ----- NO
    // ------- does the VAO bag begin after binding or after creating the VAO?------- BINDING

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    unsigned int VAO_Y, VBO_Y;
    glGenBuffers(1, &VBO_Y);
    glGenVertexArrays(1,&VAO_Y);
    glBindVertexArray(VAO_Y);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Y);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2),vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);



    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO_Y);
        glDrawArrays(GL_TRIANGLES, 0,3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* float b[2] = {1.0, 1.0};
    std::cout<<sizeof(b)<<std::endl; */
    // free memory
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}