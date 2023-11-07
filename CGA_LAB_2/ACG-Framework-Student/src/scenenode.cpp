#include "scenenode.h"
#include "application.h"
#include "texture.h"
#include "utils.h"

unsigned int SceneNode::lastNameId = 0;
unsigned int mesh_selected = 0;

SceneNode::SceneNode()
{
	this->name = std::string("Node" + std::to_string(lastNameId++));
}

SceneNode::SceneNode(const char * name)
{
	this->name = name;
}

SceneNode::~SceneNode()
{

}

void SceneNode::render(Camera* camera)
{
	if (material)
		material->render(mesh, model, camera);
}

void SceneNode::renderWireframe(Camera* camera)
{
	WireframeMaterial mat = WireframeMaterial();
	mat.render(mesh, model, camera);
}

void SceneNode::renderInMenu()
{
	//Model edit
	if (ImGui::TreeNode("Model")) 
	{
		ImGui::Checkbox("Autorotate", &Application::instance->autorotate);
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(model.m, matrixTranslation, matrixRotation, matrixScale);
		ImGui::DragFloat3("Position", matrixTranslation, 0.1f);
		ImGui::DragFloat3("Rotation", matrixRotation, 0.1f);
		ImGui::DragFloat3("Scale", matrixScale, 0.1f);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, model.m);
		
		ImGui::TreePop();
	}

	//Material
	if (material && ImGui::TreeNode("Material"))
	{
		material->renderInMenu();
		ImGui::TreePop();
	}
}

VolumeNode::VolumeNode()
{
	this->name = std::string("Node" + std::to_string(lastNameId++));
}

VolumeNode::VolumeNode(const char* name)
{
	this->name = name;

	//Mesh
	Mesh* newmesh = new Mesh();
	newmesh->createCube();
	this->mesh = newmesh;

	//Volumes
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


	//Model
	float x = vol_abdomen->width * vol_abdomen->widthSpacing;
	float y = (vol_abdomen->height * vol_abdomen->heightSpacing) / x;
	float z = (vol_abdomen->depth * vol_abdomen->depthSpacing) / x;
	this->model.scale(1.0, y, z);

	//Texture
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


	//Material
	VolumeMaterial* vol_mat = new VolumeMaterial();
	this->material = vol_mat;
	vol_mat->texture = text_abdomen;
	vol_mat->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/volumeshader.fs");

	//model material
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

}

VolumeNode::~VolumeNode()
{
}