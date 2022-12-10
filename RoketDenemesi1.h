// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MapSystem.h"
#include "RoketDenemesi1.generated.h"

UCLASS()
class ALLAHIMINSALLAH_API ARoketDenemesi1 : public APawn, public FPortListener
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoketDenemesi1();

	UPROPERTY(EditAnywhere)
		AActor* RCamera;
	UPROPERTY(EditAnywhere)
		AActor* MainCamera;

	SerialPort* Serial;
protected:
	void StopPhase1Smoke();
	void StopPhase2Smoke();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//overriden from FPortListener
	virtual void Update(const RocketData Data, EListenerType AvionicType, const float DTime) override;
};
