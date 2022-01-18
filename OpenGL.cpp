#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <cstring>
#include <stdlib.h>


const int width_window = 640;
const int height_window = 480;


float circle_center_x = 0.0;
float circle_center_y = 0.0;

//key controll
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        circle_center_x -= 0.05;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        circle_center_x += 0.05;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        circle_center_y += 0.05;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        circle_center_y -= 0.05;
    }

}

//cursor controll
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << xpos << " " << ypos << std::endl;
}

int main(void)
{
    GLFWwindow* window = nullptr;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width_window,height_window, "KyuHwan", NULL, NULL);
                                                                    //multi window
    if (!window)
    {
        glfwTerminate();
        return -1; 
    }

    //callback here
    glfwSetKeyCallback(window, key_callback); 
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor(174.0/ 255.0, 255.0 / 255.0, 255.0 / 255.0, 1);


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height); 


    const float aspect_ratio = (float)width / (float)height;
    glOrtho(-1, 1, -1.0/ aspect_ratio, 1.0/ aspect_ratio, -1.0, 1.0); //: You are going to study later in viewing class 
    // 카메라 뷰포인트를 바꾸는 것 같음..
    // 바텀 탑에 ar 을 나누어 주니 정확한 원이 되었다. 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //To do : draw here 
        glBegin(GL_TRIANGLE_FAN);

        //center of polygonized circle
        glColor3f(0, 0, 0);
        glVertex2f(circle_center_x, circle_center_y);

        const int num_triangles = 1000; 
        const float dtheta = 2.0 * 3.141592 / (float)num_triangles;
        const float radius = 0.5;

        float theta = 0.0;
        for (int i = 0; i <= num_triangles; i++,theta += dtheta)
        {
            const float x = radius * cos(theta) + circle_center_x;
            const float y = radius * sin(theta) + circle_center_y;
            //회전해가면서 삼각형을 그림 
            glVertex2f(x, y);
        }       
        glEnd();
        //note :: this is old style
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents(); // callback function

        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // save graphic card
    }

    glfwTerminate();
    return 0;
}


