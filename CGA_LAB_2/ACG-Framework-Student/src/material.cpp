#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

unsigned int item_current = 0;
unsigned int text_current = 0;

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
	noise_texture = Texture::Get("data/images/blueNoise.png");
	TF_texture = Texture::Get("data/images/TF_abdomen.png");
	TF_abdomen = Texture::Get("data/images/TF_abdomen.png");
	TF_foot = Texture::Get("data/images/TF_foot.png");
	TF_orange = Texture::Get("data/images/TF_orange.png");
	TF_daisy = Texture::Get("data/images/TF_daisy.png");
	TF_bonsai = Texture::Get("data/images/TF_bonsai.png");
	TF_teapot = Texture::Get("data/images/TF_teapot.png");
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
	shader->setUniform("u_color", u_color);
	shader->setUniform("alpha_discard", alpha_discard);
	shader->setUniform("u_step", step);
	shader->setUniform("u_brightness", brightness);
	shader->setUniform("jittering_1", jittering_1);
	shader->setUniform("jittering_2", jittering_2);
	shader->setUniform("transfer", transfer);
	shader->setUniform("TF", TF_texture);
	shader->setUniform("clipping", clipping);
	shader->setUniform("plane_clipping", plane_clipping);
	shader->setUniform("isosurface", isosurface);
	shader->setUniform("gradient_threshold", gradient_threshold);
	shader->setUniform("h", h);

	shader->setUniform("ambient_light", ambient_light);
	shader->setUniform("diffuse_light", diffuse_light);
	shader->setUniform("specular_light", specular_light);
	shader->setUniform("ka", ka);
	shader->setUniform("kd", kd);
	shader->setUniform("alpha", alpha);
	shader->setUniform("light_position", light_position);

	if (texture)
		shader->setUniform("u_texture", texture);
	if (noise_texture)
		shader->setUniform("noise_texture", noise_texture);
}

void VolumeMaterial::renderInMenu()
{

	const char* items[] = {"Abdomen", "Daisy", "Orange", "Bonsai", "Foot", "Teapot"};
	Texture* textures[] = {TF_abdomen, TF_daisy, TF_orange, TF_bonsai, TF_foot, TF_teapot};
	const char* textures_def[] = {"TF_Abdomen", "TF_Daisy", "TF_Orange", "TF_Bonsai", "TF_Foot", "TF_Teapot"};

if (item_current >= 0 && item_current < 6) {
	Texture* selectedTexture = textures[text_current >= 0 && text_current < 6 ? text_current : 0];

	//ImGui Change between textures:
	switch (item_current) {
	case 0: this->texture = texture_abdomen; break;
	case 1: this->texture = texture_daisy; break;
	case 2: this->texture = texture_orange; break;
	case 3: this->texture = texture_bonsai; break;
	case 4: this->texture = texture_foot; break;
	case 5: this->texture = texture_teapot; break;
	}

	this->TF_texture = selectedTexture;
	}

	ImGui::Combo("Choose the Volume", (int*)&item_current, items, IM_ARRAYSIZE(items));

	ImGui::ColorEdit3("Color", (float*)&u_color); // Edit 3 floats representing a color
	ImGui::SliderFloat("Step", &step, 0.0, 1.0);
	ImGui::SliderFloat("Brightness", &brightness, 0, 10);
	ImGui::SliderFloat("Alpha Discard", &alpha_discard, 0, 0.15);
	ImGui::Checkbox("Jittering [Blue-Noise Texture Approach]", &jittering_1);
	ImGui::Checkbox("Jittering [Pseudorandom-looking Function Approach]", &jittering_2);
	ImGui::Checkbox("Transfer Function", &transfer);
	ImGui::Combo("Choose the Transfer Function", (int*)&text_current, textures_def, IM_ARRAYSIZE(textures_def));
	ImGui::Checkbox("Clipping", &clipping);
	ImGui::SliderFloat3("Slider Clipping X-Y-Z", &plane_clipping.x, 0, 1);
	ImGui::SliderFloat("Slider Clipping W", &plane_clipping.w, -1, 1);
	ImGui::Checkbox("Isosurface", &isosurface);
	ImGui::SliderFloat("Gradient Threshold", &gradient_threshold, 0.0, 1.0);
	ImGui::SliderFloat("h", &h, 0.0, 1.0);
	ImGui::DragFloat3("Light position", (float*)&light_position, 0.1f);

	if (ImGui::Button("Abdomen Preset")) {
		abdomenPreset();
	}
	if (ImGui::Button("Daisy Preset")) {
		daisyPreset();
	}
	if (ImGui::Button("Orange Preset")) {
		orangePreset();
	}
	if (ImGui::Button("Bonsai Preset")) {
		bonsaiPreset();
	}
	if (ImGui::Button("Foot Preset")) {
		footPreset();
	}
	if (ImGui::Button("Teapot Preset")) {
		teapotPreset();
	}
	if (ImGui::Button("Abdomen Isosurface Preset")) {
		abdomenIsoPreset();
	}
	if (ImGui::Button("Daisy Isosurface Preset")) {
		daisyIsoPreset();
	}
	if (ImGui::Button("Orange Isosurface Preset")) {
		orangeIsoPreset();
	}
	if (ImGui::Button("Bonsai Isosurface Preset")) {
		bonsaiIsoPreset();
	}
	if (ImGui::Button("Foot Isosurface Preset")) {
		footIsoPreset();
	}
	if (ImGui::Button("Teapot Isosurface Preset")) {
		teapotIsoPreset();
	}
}

//PRESETS:
void VolumeMaterial::abdomenPreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.085;
	text_current = 0;
	item_current = 0;
}

void VolumeMaterial::daisyPreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.1;
	text_current = 1;
	item_current = 1;
}

void VolumeMaterial::orangePreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.123;
	text_current = 2;
	item_current = 2;
}

void VolumeMaterial::bonsaiPreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.0373;
	brightness = 7.5;
	alpha_discard = 0.085;
	text_current = 3;
	item_current = 3;
}

void VolumeMaterial::footPreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.09;
	text_current = 4;
	item_current = 4;
}

void VolumeMaterial::teapotPreset()
{
	isosurface = false;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.035;
	brightness = 10;
	alpha_discard = 0.035;
	text_current = 5;
	item_current = 5;
}

//ISOSURFACES:
void VolumeMaterial::abdomenIsoPreset()
{
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	isosurface = true;
	u_color = vec4(0.0, 0.059, 1.0, 1.0);
	gradient_threshold = 0.402;
	jittering_2 = true;
	transfer = false;
	clipping = true;
	step = 0.02;
	brightness = 1.73;
	alpha_discard = 0.150;
	text_current = 0;
	item_current = 0;
}

void VolumeMaterial::daisyIsoPreset()
{
	isosurface = true;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	u_color = vec4(0.088, 0.0, 1.0, 1.0);
	gradient_threshold = 0.185;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.1;
	text_current = 1;
	item_current = 1;
}

void VolumeMaterial::orangeIsoPreset()
{
	isosurface = true;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	u_color = vec4(1.0, 0.059, 0.0, 1.0);
	gradient_threshold = 0.211;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.123;
	text_current = 2;
	item_current = 2;
}

void VolumeMaterial::bonsaiIsoPreset()
{
	isosurface = true;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	u_color = vec4(0.0, 0.059, 1.0, 1.0);
	gradient_threshold = 0.236;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.0373;
	brightness = 7.5;
	alpha_discard = 0.085;
	text_current = 3;
	item_current = 3;
}

void VolumeMaterial::footIsoPreset()
{
	isosurface = true;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	u_color = vec4(0.0, 0.059, 1.0, 1.0);
	gradient_threshold = 0.421;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.02;
	brightness = 6.2;
	alpha_discard = 0.150;
	text_current = 4;
	item_current = 4;
}

void VolumeMaterial::teapotIsoPreset()
{
	isosurface = true;
	plane_clipping.x = 0.0;
	plane_clipping.y = 0.0;
	plane_clipping.z = 1.0;
	plane_clipping.w = -1.0;
	u_color = vec4(0.059, 0.0, 1.0, 1.0);
	gradient_threshold = 0.148;
	jittering_2 = true;
	transfer = true;
	clipping = true;
	step = 0.004;
	brightness = 7.3;
	alpha_discard = 0.05;
	text_current = 5;
	item_current = 5;
}