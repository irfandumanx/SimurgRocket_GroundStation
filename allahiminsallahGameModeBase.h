// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChartExample.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include <UObject/ConstructorHelpers.h>
#include "GameFramework/GameModeBase.h"
#include "allahiminsallahGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALLAHIMINSALLAH_API AallahiminsallahGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AallahiminsallahGameModeBase();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> WidgetClass;
	virtual void BeginPlay() override;
public:
	inline static UChartExample* ChartExample;
};	
