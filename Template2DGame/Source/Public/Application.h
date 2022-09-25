#pragma once
#include "Core/Actor.h"

class ACameraActor;
class UChessModel;
class UChessView;
class UChessController;

class AApplication : public AActor
{
protected:
	IMPLEMENT_CONSTRUCTOR(AApplication, AActor);

	virtual void Begin() override;
	virtual void Tick(float dt) override;
	virtual void End() override;

private:
	ACameraActor* Camera;

	UChessModel* Model;
	UChessView* View;
	UChessController* Controller;
};