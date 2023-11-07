#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

unsigned int item_current = 0;

StandardMaterial::StandardMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

StandardMaterial::~StandardMaterial()
{

}

void StandardMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_color", color);

	if (texture)
		shader->setUniform("u_texture", texture);
}

void StandardMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void StandardMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
}

WireframeMaterial::WireframeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

WireframeMaterial::~WireframeMaterial()
{

}

void WireframeMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (shader && mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

VolumeMaterial::VolumeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/volumeshader.fs");
}

VolumeMaterial::~VolumeMaterial()
{

}

void VolumeMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (shader && mesh)
	{
		//modify OpenGL flags
		glEnable(GL_BLEND);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//upload model
		if (item_current == 0) {
			setUniforms(camera, model);
		}
		else if (item_current == 1) {
			setUniforms(camera, model_daisy);
		}
		else if (item_current == 2) {
			setUniforms(camera, model_orange);
		}
		else if (item_current == 3) {
			setUniforms(camera, model_bonsai);
		}
		else if (item_current == 4) {
			setUniforms(camera, model_foot);
		}
		else if (item_current == 5) {
			setUniforms(camera, model_teapot);
		}

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();

		//restore OpenGL flags
		glDisable(GL_BLEND);
	}
}

void VolumeMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	Matrix44 invModel = model;
	invModel.inverse();
	Vector4 temp = invModel * Vector4(camera->eye, 1.0);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", invModel * camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_inverse_model", invModel);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_color", color);
	shader->setUniform("u_step", step);
	shader->setUniform("u_brightness", brightness);

	if (texture)
		shader->setUniform("u_texture", texture);
}

void VolumeMaterial::renderInMenu()
{

	const char* items[] = { "Abdomen", "Daisy", "Orange","Bonsai","Foot","Teapot" };

	if (item_current == 0) {
		this->texture = texture_abdomen;
	}
	else if (item_current == 1) {
		this->texture = texture_daisy;
	}
	else if (item_current == 2) {
		this->texture = texture_orange;
	}
	else if (item_current == 3) {
		this->texture = texture_bonsai;
	}
	else if (item_current == 4) {
		this->texture = texture_foot;
	}
	else if (item_current == 5) {
		this->texture = texture_teapot;
	}

	ImGui::Combo("Choose the Volume", (int*)&item_current, items, IM_ARRAYSIZE(items));

	//ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
	ImGui::SliderFloat("Step", &step, 0.0, 1.0);
	ImGui::SliderFloat("Brightness", &brightness, 0, 10);
}