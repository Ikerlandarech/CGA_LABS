#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "extra/hdre.h"

class Material {
public:

	Shader* shader = NULL;
	Texture* texture = NULL;
	Texture* noise_texture = NULL;
	Texture* TF_texture = NULL;
	vec4 color;

	Texture* texture_abdomen = NULL;
	Texture* texture_bonsai = NULL;
	Texture* texture_foot = NULL;
	Texture* texture_daisy = NULL;
	Texture* texture_orange = NULL;
	Texture* texture_teapot = NULL;

	Texture* TF_abdomen = NULL;
	Texture* TF_bonsai = NULL;
	Texture* TF_foot = NULL;
	Texture* TF_daisy = NULL;
	Texture* TF_orange = NULL;
	Texture* TF_teapot = NULL;

	Matrix44 model_abdomen;
	Matrix44 model_bonsai;
	Matrix44 model_foot;
	Matrix44 model_daisy;
	Matrix44 model_orange;
	Matrix44 model_teapot;

	virtual void setUniforms(Camera* camera, Matrix44 model) = 0;
	virtual void render(Mesh* mesh, Matrix44 model, Camera * camera) = 0;
	virtual void renderInMenu() = 0;
};

class StandardMaterial : public Material {
public:

	StandardMaterial();
	~StandardMaterial();

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera * camera);
	void renderInMenu();
};

class WireframeMaterial : public StandardMaterial {
public:

	WireframeMaterial();
	~WireframeMaterial();

	void render(Mesh* mesh, Matrix44 model, Camera * camera);
};

class VolumeMaterial : public StandardMaterial {
public:

	float step = 0.02;
	float brightness = 5.0;
	float alpha_discard = 0.09;
	bool jittering_1 = false;
	bool jittering_2 = false;
	bool transfer = false;
	bool clipping = false;
	bool isosurface = false;
	vec4 plane_clipping = vec4(0.0, 0.0, -0.9, 0.0);
	float gradient_threshold = 0.3;
	float h = 0.015;
	vec3 light_position = vec3(1.0, 3.0, 5.0);
	vec3 ambient_light = vec3(1.0, 1.0, 1.0);
	vec3 diffuse_light = vec3(1.0, 1.0, 1.0);
	vec3 specular_light = vec3(1.0, 1.0, 1.0);;
	vec3 ka = vec3(1.0, 1.0, 1.0);
	vec3 kd = vec3(1.0, 1.0, 1.0);
	float alpha = 20.0;

	VolumeMaterial();
	~VolumeMaterial();

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera* camera);
	void renderInMenu();
	void abdomenPreset();
	void daisyPreset();
	void orangePreset();
	void bonsaiPreset();
	void footPreset();
	void teapotPreset();

};
#endif