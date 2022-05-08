#pragma once

#include "Scene.h"

class Title : Scene
{
public:
	Title(string name);
	virtual ~Title();

private:
	virtual void Render() override;
};

