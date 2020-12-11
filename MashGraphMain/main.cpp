// Pogosbekyan Maria 305 CMC - OpenGL Project

#include <iostream>
#include <cmath>

// GLEW 
#define GLEW_STATIC // use static version of library GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM - math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//SOIL - library for textures
#include <SOIL/SOIL.h>
// Headers
#include "Shader.h"
#include "Camera.h"

using namespace std;
using namespace glm;

// ----- Functions -----
void KeyClick(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMode); // when use keyboard
void MouseClick(GLFWwindow* window, double dX_pos, double dY_pos); // when use mouse
void Scrolling(GLFWwindow* window, double xoffset, double yoffset); // when scroll mouse
void DoMove(); // update camera coordinates when keys pressed

bool keys[1024];

// Settigs for window
const GLuint WIDTH = 1500, HEIGHT = 900;

// Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
GLfloat lastMouse_X = WIDTH / 2.0; // last coordinates of mouse - X
GLfloat lastMouse_Y = HEIGHT / 2.0; // last coordinates of mouse - Y
bool bFirstMouse = true; // first press mouse

GLuint LoadTexture(string name); // load texture

GLfloat deltaTime = 0.0f;	// time between the last frame and the current frame 
GLfloat lastFrame = 0.0f; // time of last output frame

vec3 lightPos(1.5f, 1.0f, 0.2f); // position of light source in world space


int main()
{
    // Initialization GLFW
    glfwInit();
    // Set GLFW, version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // disable resizing of the window

    // create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MASHGRAPH", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, KeyClick); // called when keyboard is moved
    glfwSetCursorPosCallback(window, MouseClick); // called when mouse is moved
    glfwSetScrollCallback(window, Scrolling); // called when mouse wheel is scrolled

    // Set GLFW
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;
    // Initialization GLEW
    glewInit();
    
    // Set coordinates relative window: Lower left corner and upper right corner
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST); // 
    glDepthFunc(GL_LESS); //
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // array of vertices
    GLfloat cubeVertices[] = {
        // positions          // normals           // texture coords
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // floor
    GLfloat planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        /* 5.0f, -0.5f,  5.0f, 0.0f,  1.0f,  1.0f,               2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f,              0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,   0.0f, 1.0f, 0.0f,             0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f,  1.0f,  0.0f, 0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  1.0f, 2.0f, 2.0f
         */
          5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   0.0f,  1.0f,
         5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f,   1.0f,  1.0f

    };
    
    /*
    // positions all cubes
    vec3 cubePositions[] = {
        vec3(0.0f,  0.0f,  0.0f),
        vec3(2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
    };

    // positions of the point lights
    vec3 pointLightPositions[] = {
        vec3(0.7f,  0.2f,  2.0f),
        vec3(2.3f, -3.3f, -4.0f),
        vec3(-4.0f,  2.0f, -12.0f),
        vec3(0.0f,  0.0f, -3.0f)
    };
    */


    // Compile shader program
    Shader cubeShader("cubeShader.vert","cubeShader.frag");
    Shader colorShader("cubeShader.vert", "colorShader.frag");
    Shader lampShader("lampShader.vert","lampShader.frag");

    // Maps for cubes
    GLuint diffuseMap = LoadTexture("art6.jpg");
    GLuint specularMap = LoadTexture("map_art6_specular.jpg");

    // Maps for floor
    GLuint diffFloorTexture = LoadTexture("hex.jpg");
    GLint specFloorTexture = LoadTexture("hex.jpg");


    // Vertex buffer objects, vertex array object
    GLuint cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO); // create VBO
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); // bind 2 buffers (VBO ê GL_Array_Buffer)
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW); // copy vertex data to buffer
    
    // Set attributes
    // Pozition attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // floor
    GLuint planeVBO, planeVAO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO); // create VBO
    glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVBO); // bind 2 buffers (VBO ê GL_Array_Buffer)
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW); // copy vertex data to buffer

    // Set attributes
    // Pozition attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    // lamp
    GLuint lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // cube settings
    cubeShader.Use();
    //cubeShader.setInt("texture1", 0);
    cubeShader.setInt("diffuse", 0);
    cubeShader.setInt("specular", 1);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate  new value of deltaTime - time between the last frame and the current frame 
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check keybord input/mouse movement
        glfwPollEvents();
        DoMove();

        // Set color of background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // paint

        /*
        // cube
        cubeShader.Use();
        cubeShader.setVec3("viewPos", camera.Position);
        cubeShader.setFloat("shininess", 32.0f);

        // directional light
        cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        cubeShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        // point light 1
        cubeShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        cubeShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[0].constant", 1.0f);
        cubeShader.setFloat("pointLights[0].linear", 0.09);
        cubeShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        cubeShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        cubeShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[1].constant", 1.0f);
        cubeShader.setFloat("pointLights[1].linear", 0.09);
        cubeShader.setFloat("pointLights[1].quadratic", 0.032);
        /*
        // point light 3
        cubeShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        cubeShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[2].constant", 1.0f);
        cubeShader.setFloat("pointLights[2].linear", 0.09);
        cubeShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        cubeShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        cubeShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[3].constant", 1.0f);
        cubeShader.setFloat("pointLights[3].linear", 0.09);
        cubeShader.setFloat("pointLights[3].quadratic", 0.032);
        
        // spotLight
        cubeShader.setVec3("spotLight.position", camera.Position);
        cubeShader.setVec3("spotLight.direction", camera.Front);
        cubeShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("spotLight.constant", 1.0f);
        cubeShader.setFloat("spotLight.linear", 0.09);
        cubeShader.setFloat("spotLight.quadratic", 0.032);
        cubeShader.setFloat("spotLight.cutOff", cos(radians(12.5f)));
        cubeShader.setFloat("spotLight.outerCutOff", cos(radians(15.0f)));
        */

        
        colorShader.Use();

        // view/projection transformations
        // Calculate and send to shader matrix of perspective proection with field of view angle
        mat4 projection = perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        colorShader.setMat4("projection", projection);
        colorShader.setMat4("view", view);
        // world transformation
        mat4 model = mat4(1.0f);
        
        // cube
        cubeShader.Use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        // settings for model Fong
        cubeShader.setVec3("light.position", lightPos);
        cubeShader.setVec3("viewPos", camera.Position);
        cubeShader.setFloat("material.shininess", 32.0f);
        cubeShader.setVec3("light.ambient", 0.45f, 0.45f, 0.45f);
        cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("light.constant", 1.0f);
        cubeShader.setFloat("light.linear", 0.0002f);
        cubeShader.setFloat("light.quadratic", 0.0002f);


        //cubeShader.Use();
        // ----- Effect Obvodka ------
        glStencilMask(0x00);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffFloorTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specFloorTexture);
        cubeShader.setMat4("model", mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        // First render pass, draw objects as normal, writing to the stencil buffer
        // --------------------------------------------------------------------
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // each fragment will update the stencil buffer
        glStencilMask(0xFF); // enable writing to the stencil buffer

        // cubes
        // textures activation and binding
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap); // cubeTexture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        
        model = translate(model, vec3(-1.0f, 0.0f, -1.0f));
        cubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = mat4(1.0f);
        model = translate(model, vec3(2.0f, 0.0f, 0.0f));
        cubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Second render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
        // the objects' size differences, making it look like borders.
        // -----------------------------------------------------------------------------------------------------------------------------
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // each bit becomes zero in the stencil buffer (stop recording)
        glDisable(GL_DEPTH_TEST);
        colorShader.Use();
        GLfloat scale = 1.1;
        
        // cubes
        model = mat4(1.0f);
        model = translate(model, vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, vec3(scale, scale, scale));
        colorShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = mat4(1.0f);
        model = translate(model, vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, vec3(scale, scale, scale));
        colorShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF); // each fragment will update the stencil buffer
        glEnable(GL_DEPTH_TEST);
        // ------ Effect Obvodka ------

    
        // also draw the lamp object
        lampShader.Use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = mat4(1.0f);
        model = translate(model, lightPos);
        model = glm::scale(model, vec3(0.5f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       

        // swap buffers
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;


}



// ----- KeyClick -----
// track which keys were pressed or released
void KeyClick(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMode)
{
    // if pressed ESCAPE or PRESS then close the window
    if ((iKey == GLFW_KEY_ESCAPE) && (iAction == GLFW_PRESS))
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (iKey >= 0 && iKey < 1024)
    {
        if (iAction == GLFW_PRESS)
            keys[iKey] = true;
        else if (iAction == GLFW_RELEASE)
            keys[iKey] = false;
    }
}

// ----- MouseClick -----
// check when mouse clicked
void MouseClick(GLFWwindow* window, double dX_pos, double dY_pos)
{
    if (bFirstMouse)
    {
        lastMouse_X = dX_pos;
        lastMouse_Y = dY_pos;
        bFirstMouse = false;
    }

    GLfloat xoffset = dX_pos - lastMouse_X;
    GLfloat yoffset = lastMouse_Y - dY_pos;
    lastMouse_X = dX_pos;
    lastMouse_Y = dY_pos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// ----- Scrolling -----
// check when mouse scroll
void Scrolling(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// ----- DoMove -----
// Update camera coordinates when pressed key
void DoMove()
{
    // Check what key pressed
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// ----- LoadTexture -----
GLuint LoadTexture(string name) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height;
    unsigned char* data = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}