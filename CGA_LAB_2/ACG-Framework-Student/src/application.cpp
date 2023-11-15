#include "application.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "volume.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "extra/hdre.h"
#include "extra/imgui/imgui.h"
#include "extra/imgui/imgui_impl_sdl.h"
#include "extra/imgui/imgui_impl_opengl3.h"

#include <cmath>

bool render_wireframe = false;
Camera* Application::camera = nullptr;
Application* Application::instance = NULL;

Application::Application(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	render_debug = true;
	autorotate = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	// OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	// Create camera
	camera = new Camera();
	camera->lookAt(Vector3(5.f, 5.f, 5.f), Vector3(0.f, 0.0f, 0.f), Vector3(0.f, 1.f, 0.f));
	camera->setPerspective(45.f, window_width/(float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	{
		VolumeNode* volumeNode = new VolumeNode("Volume Node");

		//Creating the mesh:
		Mesh* newmesh = new Mesh();
		newmesh->createCube();
		volumeNode->mesh = newmesh;

		//Creating all the volumes (Imgui combo)
		Volume* vol_abdomen = new Volume();
		vol_abdomen->loadPVM("data/volumes/CT-Abdomen.pvm");
		Volume* vol_foot = new Volume();
		vol_foot->loadPNG("data/volumes/foot_16_16.png", 16, 16);
		Volume* vol_teapot = new Volume();
		vol_teapot->loadPNG("data/volumes/teapot_16_16.png", 16, 16);
		Volume* vol_bonsai = new Volume();
		vol_bonsai->loadPNG("data/volumes/bonsai_16_16.png", 16, 16);
		Volume* vol_daisy = new Volume();
		vol_daisy->loadPVM("data/volumes/Daisy.pvm");
		Volume* vol_orange = new Volume();
		vol_orange->loadPVM("data/volumes/Orange.pvm");


		//Model fit:
		float x = vol_abdomen->width * vol_abdomen->widthSpacing;
		float y = (vol_abdomen->height * vol_abdomen->heightSpacing) / x;
		float z = (vol_abdomen->depth * vol_abdomen->depthSpacing) / x;
		volumeNode->model.scale(1.0, y, z);


		//Creating the textures:
		Texture* text_abdomen = new Texture();
		text_abdomen->create3DFromVolume(vol_abdomen, GL_CLAMP_TO_EDGE);
		Texture* text_foot = new Texture();
		text_foot->create3DFromVolume(vol_foot, GL_CLAMP_TO_EDGE);
		Texture* text_teapot = new Texture();
		text_teapot->create3DFromVolume(vol_teapot, GL_CLAMP_TO_EDGE);
		Texture* text_bonsai = new Texture();
		text_bonsai->create3DFromVolume(vol_bonsai, GL_CLAMP_TO_EDGE);
		Texture* text_daisy = new Texture();
		text_daisy->create3DFromVolume(vol_daisy, GL_CLAMP_TO_EDGE);
		Texture* text_orange = new Texture();
		text_orange->create3DFromVolume(vol_orange, GL_CLAMP_TO_EDGE);

		//Assigning all textures to the volume material:
		VolumeMaterial* vol_mat = new VolumeMaterial();
		volumeNode->material = vol_mat;
		vol_mat->texture = text_abdomen;
		vol_mat->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/volumeshader.fs");
		vol_mat->texture_abdomen = text_abdomen;
		vol_mat->texture_foot = text_foot;
		vol_mat->texture_teapot = text_teapot;
		vol_mat->texture_bonsai = text_bonsai;
		vol_mat->texture_daisy = text_daisy;
		vol_mat->texture_orange = text_orange;

		//Changing all model material for each model:
		x = vol_abdomen->width * vol_abdomen->widthSpacing;
		y = (vol_abdomen->height * vol_abdomen->heightSpacing) / x;
		z = (vol_abdomen->depth * vol_abdomen->depthSpacing) / x;
		vol_mat->model_abdomen.scale(1.0, y, z);

		x = vol_foot->width * vol_foot->widthSpacing;
		y = (vol_foot->height * vol_foot->heightSpacing) / x;
		z = (vol_foot->depth * vol_foot->depthSpacing) / x;
		vol_mat->model_foot.scale(1.0, y, z);

		x = vol_teapot->width * vol_teapot->widthSpacing;
		y = (vol_teapot->height * vol_teapot->heightSpacing) / x;
		z = (vol_teapot->depth * vol_teapot->depthSpacing) / x;
		vol_mat->model_teapot.scale(1.0, y, z);

		x = vol_bonsai->width * vol_bonsai->widthSpacing;
		y = (vol_bonsai->height * vol_bonsai->heightSpacing) / x;
		z = (vol_bonsai->depth * vol_bonsai->depthSpacing) / x;
		vol_mat->model_bonsai.scale(1.0, y, z);

		x = vol_daisy->width * vol_daisy->widthSpacing;
		y = (vol_daisy->height * vol_daisy->heightSpacing) / x;
		z = (vol_daisy->depth * vol_daisy->depthSpacing) / x;
		vol_mat->model_daisy.scale(1.0, y, z);

		x = vol_orange->width * vol_orange->widthSpacing;
		y = (vol_orange->height * vol_orange->heightSpacing) / x;
		z = (vol_orange->depth * vol_orange->depthSpacing) / x;
		vol_mat->model_orange.scale(1.0, y, z);

		node_list.push_back(volumeNode);
	}
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

// what to do when the image has to be draw
void Application::render(void)
{
	// set the clear color (the background color)
	glClearColor(0.1, 0.1, 0.1, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera as default
	camera->enable();

	// set flags
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	for (size_t i = 0; i < node_list.size(); i++) {
		node_list[i]->render(camera);

		if(render_wireframe)
			node_list[i]->renderWireframe(camera);
	}

	//Draw the floor grid
	if(render_debug)
		drawGrid();
}

void Application::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant
	float orbit_speed = seconds_elapsed * 1;

	// example
	float angle = seconds_elapsed * 10.f * DEG2RAD;
	if (autorotate)
	{
		for (int i = 0; i < node_list.size(); i++) {
			node_list[i]->model.rotate(angle, Vector3(0, 1, 0));
		}
	}

	// mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT && !ImGui::IsAnyWindowHovered()
		&& !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive())) //is left button pressed?
	{
		camera->orbit(-Input::mouse_delta.x * orbit_speed, Input::mouse_delta.y * orbit_speed);
	}

	// async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_Q) || Input::isKeyPressed(SDL_SCANCODE_SPACE)) camera->moveGlobal(Vector3(0.0f, -1.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_E) || Input::isKeyPressed(SDL_SCANCODE_LCTRL)) camera->moveGlobal(Vector3(0.0f, 1.0f, 0.0f) * speed);

	// to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

// Keyboard event handler (sync input)
void Application::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: render_debug = !render_debug; break;
		case SDLK_F2: render_wireframe = !render_wireframe; break;
		case SDLK_F5: Shader::ReloadAll(); break;
	}
}

void Application::onKeyUp(SDL_KeyboardEvent event)
{
}

void Application::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Application::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Application::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Application::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Application::onMouseWheel(SDL_MouseWheelEvent event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
	{
		if (event.x > 0) io.MouseWheelH += 1;
		if (event.x < 0) io.MouseWheelH -= 1;
		if (event.y > 0) io.MouseWheel += 1;
		if (event.y < 0) io.MouseWheel -= 1;
	}
	}

	if (!ImGui::IsAnyWindowHovered() && event.y)
		camera->changeDistance(event.y * 0.5);
}

void Application::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

void Application::onFileChanged(const char* filename)
{
	Shader::ReloadAll();
}

void Application::renderInMenu() {

	if (ImGui::TreeNode("Scene")) {
		//
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Camera")) {
		camera->renderInMenu();
		ImGui::TreePop();
	}

	//Scene graph
	if (ImGui::TreeNode("Entities"))
	{
		unsigned int count = 0;
		std::stringstream ss;
		for (auto& node : node_list)
		{
			ss << count;
			if (ImGui::TreeNode(node->name.c_str()))
			{
				node->renderInMenu();
				ImGui::TreePop();
			}
			++count;
			ss.str("");
		}
		ImGui::TreePop();
	}

	ImGui::Checkbox("Render debug", &render_debug);
	ImGui::Checkbox("Wireframe", &render_wireframe);
}