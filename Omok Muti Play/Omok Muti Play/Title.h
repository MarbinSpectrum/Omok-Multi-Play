#pragma once

#include "Scene.h"

class Title : public Scene
{
public:
	Title(std::string name);
	virtual ~Title();

private:
	virtual void Render() override;
};

