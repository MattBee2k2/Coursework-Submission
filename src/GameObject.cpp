#include "GameObject.h"
#include "Shader.h"
#include "Material.h"

GameObject::GameObject()
{
	m_ModelMatrix = mat4(1.0f);
	m_Position = vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = vec3(0.0f, 0.0f, 0.0f); 
	m_Scale = vec3(1.0f, 1.0f, 1.0f);

	m_ParentGameObject = NULL;
	m_ChildGameObjects.clear();

	m_Mesh = shared_ptr<Mesh>(new Mesh);
	m_Material = shared_ptr<Material>(new Material);

}


GameObject::~GameObject()
{
	m_ChildGameObjects.clear();
	m_Material->~Material();
	m_Mesh->~Mesh();
}

void GameObject::update()
{
	mat4 translationMatrix = translate(mat4(1.0f), m_Position);
	mat4 scaleMatrix = scale(mat4(1.0f), m_Scale);
	mat4 rotationMatrix = rotate(mat4(1.0f), m_Rotation.x, vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), m_Rotation.y, vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), m_Rotation.z, vec3(0.0f, 0.0f, 1.0f));
	
	mat4 parentModel = mat4(1.0f);
	if (m_ParentGameObject)
	{
		parentModel = m_ParentGameObject->getModelMatrix();
	}

	m_ModelMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	m_ModelMatrix *= parentModel;

	for (auto iter = m_ChildGameObjects.begin(); iter !=  m_ChildGameObjects.end(); iter++)
	{
		(*iter)->update();
	}
}

void GameObject::createBuffer(Vertex *pVerts, int numVerts, int *pIndices, int numIndices)
{
	m_Mesh = shared_ptr<Mesh>(new Mesh);
	m_Mesh->create(pVerts, numVerts, pIndices, numIndices);
}

void GameObject::loadShader(const string& vsFilename, const string& fsFilename)
{
	m_Material->loadShader(vsFilename, fsFilename);
}

void GameObject::setUpGameObjectMaterial()
{
	m_Material->setUpUniforms();
}

void GameObject::loadDiffuseMap(const string& filename)
{
	m_Material->loadDiffuseMap(filename);
}
