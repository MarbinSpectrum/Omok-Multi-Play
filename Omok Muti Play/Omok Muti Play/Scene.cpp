#include "Scene.h"

Scene::Scene(string name) : sceneName(name)
{
	//������
}

Scene::~Scene()
{
	//�Ҹ���
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���̸��� �����´�.
////////////////////////////////////////////////////////////////////////////////////////////////
string Scene::GetSceneName()
{
	return sceneName;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// : ���� �������Ѵ�.
////////////////////////////////////////////////////////////////////////////////////////////////
void Scene::Render()
{

}