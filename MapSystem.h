// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "allahiminsallahGameModeBase.h"
#include "jni.h"

/**
 * 
 */
class ALLAHIMINSALLAH_API MapSystem : public FRunnable, public FPortListener
{
public:
	MapSystem();

	virtual bool Init() override;
	virtual void Stop() override;
	virtual uint32 Run() override;

	//overriden from FPortListener
	virtual void Update(const RocketData Data, EListenerType AvionicType, const float DTime) override;
private:
	FRunnableThread* thread;
};
