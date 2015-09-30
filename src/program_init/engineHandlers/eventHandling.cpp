#define GLEW_STATIC

//#include "../mainHeaders.h"
#include "../EngineHeader.h"

using namespace std;


//-------------------------------------------//
//            Input Class Functions          //
//-------------------------------------------//
//Function handles all press and hold movements
void Engine_Init::Do_Movement()
{
        if (consoleRequest == 0)
        {
    		// Camera controls
    		GLfloat cameraSpeed;
    		if(keys[GLFW_KEY_LEFT_SHIFT])
        		{cameraSpeed = 500.0f * dt.t;}
    		else
        		{cameraSpeed = 20.0f * dt.t;}

    		//cout <<  keys[0] << endl;

    		if(keys[GLFW_KEY_W])
        		{camera.Pos += cameraSpeed * camera.Dir;}
    		if(keys[GLFW_KEY_S])
        		{camera.Pos -= cameraSpeed * camera.Dir;}
    		if(keys[GLFW_KEY_A])
        		{camera.Pos -= glm::normalize(glm::cross(camera.Dir, camera.Up)) * cameraSpeed;}
    		if(keys[GLFW_KEY_D])
     			{camera.Pos +=  glm::normalize(glm::cross(camera.Dir, camera.Up)) * cameraSpeed;}
		

                /*if(keys[GLFW_KEY_W])
                {
			glm::vec2 camVel = glm::vec2(camera.Dir.x,camera.Dir.z);
			camVel = glm::normalize(camVel);
			camera.Pos += glm::vec3(camVel.x * cameraSpeed,0.0f,camVel.y * cameraSpeed);
			//cout << "CAMPOS+: [" << camera.Pos.x << "," << camera.Pos.x << "," << camera.Pos.z 
			 //<< "] camera.Vel [" << camVel.x << "," << camVel.y << "," << camVel.z
			 //<< "] camera.Dir [" << camera.Dir.x << "," << camera.Dir.y << "," << camera.Dir.z << endl;
		}
                if(keys[GLFW_KEY_S])
                {
                        glm::vec2 camVel = glm::vec2(camera.Dir.x,camera.Dir.z);
                        camVel = glm::normalize(camVel);
                        camera.Pos -= glm::vec3(camVel.x * cameraSpeed,0.0f,camVel.y * cameraSpeed);
                        //cout << "CAMPOS+: [" << camera.Pos.x << "," << camera.Pos.x << "," << camera.Pos.z 
                        //<< "] camera.Vel [" << camVel.x << "," << camVel.y << "," << camVel.z 
                        //<< "] camera.Dir [" << camera.Dir.x << "," << camera.Dir.y << "," << camera.Dir.z << endl;
		}
                if(keys[GLFW_KEY_A])
                	{camera.Pos -= glm::normalize(glm::cross(camera.Dir, camera.Up)) * cameraSpeed;}
                if(keys[GLFW_KEY_D])
                        {camera.Pos +=  glm::normalize(glm::cross(camera.Dir, camera.Up)) * cameraSpeed;}
		*/
	}
}

//Function handles key callbacks
void Engine_Init::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
        // When a user presses the escape key, we set the WindowShouldClose property to true, 
        // closing the application
	if (consoleRequest == 0)
	{
	        if(action == GLFW_PRESS)
	                keys[key] = true;
	        else if(action == GLFW_RELEASE)
	                keys[key] = false;

        	if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
        	{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			consoleRequest = 1;
		}

		//cout << "key: " << key << " action: " << action << endl;

                if(key == 45 && action == 1)//"-" "press" 
                	{dt.mult = dt.mult * 0.5f;}

                if(key == 61 && action == 1)//"+" "press" 
                        {dt.mult = dt.mult * 2.0f;}

                if(key == GLFW_KEY_K && action == 1)//"K" "press" 
                        {offsetNormalization = offsetNormalization * 0.5f;cout << "ON: " << offsetNormalization << endl;}

                if(key == GLFW_KEY_L && action == 1)//"L" "press" 
                        {offsetNormalization = offsetNormalization * 2.0f;cout << "ON: " << offsetNormalization << endl;}

        	//if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        	//        glfwSetWindowShouldClose(window, GL_TRUE);
	} else {
		ConsoleFunctions(key,action);
	}
};

//Function handles mouse call backs
void Engine_Init::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (consoleRequest == 0)
    {
    	if(firstMouse)
    	{
    	    camera.lastX = xpos;
    	    camera.lastY = ypos;
    	    firstMouse = false;
    	}

    	GLfloat xoffset = xpos - camera.lastX;
    	GLfloat yoffset = camera.lastY - ypos;
    	camera.lastX = xpos;
    	camera.lastY = ypos;

    	GLfloat sensitivity = 0.05;
    	xoffset *= sensitivity;
    	yoffset *= sensitivity;

    	camera.yaw += xoffset;
    	camera.pitch += yoffset;
	
	    if(camera.pitch > 89.0f)
	       camera.pitch = 89.0f;
	    if(camera.pitch < -89.0f)
	       camera.pitch = -89.0f;
	
	//	cout << "PITCH: " << camera.pitch << endl;
	
	    glm::vec3 front;
	    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	    front.y = sin(glm::radians(camera.pitch));
	    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	    camera.Dir = glm::normalize(front);
	}
};

//Function handles mouse scroll callbacks
void Engine_Init::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
        if (consoleRequest == 0)
        {
		if(camera.aspect >= 1.0f && camera.aspect <= 45.0f)
        		camera.aspect -= yoffset * 0.1;
  		if(camera.aspect <= 1.0f)
        		camera.aspect = 1.0f;
		if(camera.aspect >= 45.0f)
        		camera.aspect = 45.0f;
		//ZoomScale += yoffset * 1.0f;
		//cout << "ZoomScale: " << ZoomScale << " PS: " << camera.PosScaled.x << "," << camera.PosScaled.y << "," << camera.PosScaled.z << endl;
	}
	
};

