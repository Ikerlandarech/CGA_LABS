#include "volumenode.h"
#include "application.h"
#include "texture.h"
#include "utils.h"

unsigned int VolumeNode::lastNameId = 0;
unsigned int mesh_selected = 0;

VolumeNode::VolumeNode()
{
	this->name = std::string("Node" + std::to_string(lastNameId++));
}

VolumeNode::VolumeNode(const char * name)
{
	this->name = name;
}

VolumeNode::~VolumeNode()
{

}

void VolumeNode::render(Camera* camera)
{
	if (material)
		material->render(mesh, model, camera);
}

void VolumeNode::renderWireframe(Camera* camera)
{
	WireframeMaterial mat = WireframeMaterial();
	mat.render(mesh, model, camera);
}

void VolumeNode::renderInMenu()
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