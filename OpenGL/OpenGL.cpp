#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glfw3.h>
#include <glew.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float velocity[] = { 0.0018f, 0.0016f };
float speed = 5;

const char* vertexShaderSource = 
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
"layout(location = 1) in vec3 aColor;\n" // the color variable has attribute position 1
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n" // specify a color output to the fragment shader
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n" // see how we directly give a vec3 to vec4's constructor
"   ourColor = aColor;\n" // set the output variable to a dark-red color
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
//"   FragColor = vec4(ourColor, 1.0);\n"
"   FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
"}\0";


//float baseTriangleSmall[] = {
//    -0.5f, 0.0f, 0.0f,
//    -0.25f, 0.5f, 0,
//    0.f, 0.f, 0.f,
//};
//
//float triangles[] =
//{
//    -0.5f, 0.0f, 0.0f,
//    -0.25f, 0.25f, 0,
//    0.f, 0.f, 0.f,
//    0.5f, 0.0f, 0.0f,
//    0.25f, 0.25f, 0
//};
//
//float squareVertices[] = {
//     0.25f,  0.25f, 0.0f,  // top right
//     0.25f, -0.25f, 0.0f,  // bottom right
//    -0.25f, -0.25f, 0.0f,  // bottom left
//    -0.25f,  0.25f, 0.0f   // top left 
//};
//float texCords[] = {
//    0.0f, 0.0f,
//    1.0f, 0.0f,
//    0.0f, 1.0f,
//    1.0f, 1.0f
//};

float squareVerticesWithColor[] = {
    //#position           #Color              # texCoords
     0.1f,  0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.1f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.1f,  0.1f, 0.0f,   0.2f, 0.0f, 0.5f,   0.0f, 1.0f // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

// Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // dash
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        speed = 10;
    }
    else
    {
        speed = 5;
    }
}

void moveShape(float* t, float vertices, float posX, float posY)
{
    for (size_t i = 0; i < vertices; i++)
    {
        // pos
        t[i * 8] += posX;
        t[(i * 8) + 1] += posY;
        // Color
        float colorValue = posX + posY;
        t[(i * 8) + 3] += colorValue;
        t[(i * 8) + 4] += colorValue;
        t[(i * 8) + 5] += colorValue;
    }
}

void dvdBounce(float* t)
{
    moveShape(t, 4, velocity[0] * speed, velocity[1] * speed);
    if (t[0] > 1)
    {
        velocity[0] *= -1;
    }
    else if (t[16] < -1)
    {
        velocity[0] *= -1;
    }
    if (t[1] > 1 || t[17] < -1)
    {
        velocity[1] = -velocity[1];
    }
}

int main()
{
    // ######################################
    //      Init glfw and glew
    // ######################################
    if (!glfwInit())
    {
        std::cout << "glfw failed to initialize";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Poggers", NULL, NULL);
    if (!window)
    {
        std::cout << "Window creation failed!";
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew failed to initialize!";
        return -1;
    }


    // ###########################
    //       shaders
    // ###########################
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int  success; // Error handeling
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Debugging
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING::FAILED\n" << infoLog << std::endl;;
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader); // Delete compiled shaders
    glDeleteShader(fragmentShader);

    // Parameters( Position of atribute, number of values, type of value, if value is normalized, space between the vertex atributes in data, position of data in the buffer)
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // Vertex buffer id
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // #####################################
    //         Textures
    // #####################################
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("newDvd.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Start ofsset of box
    moveShape(squareVerticesWithColor, 4, velocity[0] * 500.f, velocity[1] * -20.f);

    // ###############################
    //         Main Loop
    // ###############################
    while (!glfwWindowShouldClose(window)) // Main loop
    {
        // ### Input
        processInput(window);

        // ### Rendering
        // shader
        /*
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLoacation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLoacation, 0.0f, greenValue, 0.0f, 1.0f);
        */
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        dvdBounce(squareVerticesWithColor);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerticesWithColor), squareVerticesWithColor, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 3, 5);
        glBindVertexArray(0);


        // ### Check and call evends and swap the buffers
        glfwSwapBuffers(window); // Swaps the color buffer
        glfwPollEvents(); // events like keyboard input
    }

    glfwTerminate();

    return 0;
}