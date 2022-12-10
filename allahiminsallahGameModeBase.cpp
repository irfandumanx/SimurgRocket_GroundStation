// Copyright Epic Games, Inc. All Rights Reserved.


#include "allahiminsallahGameModeBase.h"

AallahiminsallahGameModeBase::AallahiminsallahGameModeBase()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>
		ChartWidget(TEXT("/Game/StarterContent/BP_ChartDeneme"));

	if (!ensure(ChartWidget.Class != nullptr)) return;
	WidgetClass = ChartWidget.Class;
}


void AallahiminsallahGameModeBase::BeginPlay()
{
	ChartExample = 
		CreateWidget<UChartExample>(GetWorld(), WidgetClass);
	if (ChartExample == nullptr) return;

	ChartExample->AddToViewport();

}
