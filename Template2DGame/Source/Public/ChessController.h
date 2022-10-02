#pragma once
#include "Core/Object.h"
#include <glm/glm.hpp>

class UChessModel;

class UChessController : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UChessController, UObject);

	virtual void Begin() override;

public:
	virtual void UpdateController(UChessModel* Model);
	glm::vec3 GetCursorWorldPosition() const;

private:
	bool bLastMouseState;

	unsigned int MouseDownSquare;
};