#ifndef VOLUMENODE_H
#define VOLUMENODE_H

#include "framework.h"

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "material.h"
#include "volume.h"

class Light;

class VolumeNode {
public:

	static unsigned int lastNameId;

	VolumeNode();
	VolumeNode(const char* name);
	~VolumeNode();

	Material * material = NULL;
	std::string name;

	Mesh* mesh = NULL;
	Matrix44 model;

	virtual void render(Camera* camera);
	virtual void renderWireframe(Camera* camera);
	virtual void renderInMenu();
};

#endif