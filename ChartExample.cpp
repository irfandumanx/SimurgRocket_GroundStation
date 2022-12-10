// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartExample.h"

// 100px 200 metre leftStick
// under 500px 10sn koysam 100 px arasi 2sn
void UChartExample::NativeConstruct()
{
	Super::NativeConstruct();
	FPortObserver::Attach(this);
	int32 LeftArrayVal[] = { 800, 600, 400, 200, 0 };
	LeftStickValues.Append(LeftArrayVal, ARRAY_COUNT(LeftArrayVal));
	int32 UnderArrayVal[] = { 0, 2, 4, 6, 8, 10 };
	UnderStickValues.Append(UnderArrayVal, ARRAY_COUNT(UnderArrayVal));

}

void UChartExample::NativeDestruct()
{
	FPortObserver::Detach(this);
}

/*
int UniversalXEffect = 0;
int UniversalYEffect = 0;
*/
int OldCenteredPosition = 400;
int CounterComp = 10;
int InCounter;
void UChartExample::Update(const RocketData Data, EListenerType AvionicType, const float DTime)
{
	if (AvionicType != MainAvionic) return;
	if (DTime == -1) return;

	SetCounter(GetCounter() + DTime);
	InCounter = static_cast<int>(GetCounter());

	float FakeCounter = GetCounter();

	if (InCounter >= 10)
	{
		FakeCounter = FMath::Fmod(FakeCounter, 10);
		FakeCounter = FMath::Fmod(FakeCounter, 2);

		FakeCounter += 8;
	}


	if (InCounter % 2 == 0 && InCounter >= CounterComp)
	{
		InCounter += 2;
		CounterComp = InCounter;
		LinesArray.RemoveAll([](const FVector2D Vector)
			{
				return Vector.X >= 1480 && Vector.X < 1580;
			});

		for (int64 Index = 0; Index != LinesArray.Num(); ++Index)
		{
			FVector2D Vector = LinesArray[Index];
			LinesArray[Index] = FVector2D(Vector.X - 100, Vector.Y);
		}

		UnderStickValues.RemoveAt(0);
		UnderStickValues.Add(InCounter);
	}


	FakeCounter = ((FakeCounter * 10) * 5) + 1480;
	float AltitudeGraph = 420 - ((Data.MainData.altitude - OldCenteredPosition + 400) / 2.0f);
	LinesArray.Add(FVector2D(FakeCounter, AltitudeGraph));


	LeftStickValues.Reset();
	int CenteredPosition = static_cast<int>(Data.MainData.altitude / 200) * 200;
	if (CenteredPosition < 400) CenteredPosition = 400;
	for (int i = 4; i >= 0; i--) {
		if (i == 2) continue;
		LeftStickValues.Add(CenteredPosition + ((i - 2) * 200));
	}
	LeftStickValues.Insert(CenteredPosition, 2);
	if (OldCenteredPosition != 0 && CenteredPosition != OldCenteredPosition)
	{
		const bool InFall = OldCenteredPosition > CenteredPosition;
		for (int64 Index = 0; Index != LinesArray.Num(); ++Index)
		{
			FVector2D Vector = LinesArray[Index];
			LinesArray[Index] = FVector2D(Vector.X, Vector.Y +
				(OldCenteredPosition > CenteredPosition ? -100 : +100));
		}
		LinesArray.RemoveAll([&](const FVector2D Vector)
			{
				return Vector.Y < 20 || Vector.Y > 420;
			});

	}
	OldCenteredPosition = CenteredPosition;
	/*
	AallahiminsallahGameModeBase::ChartExample->SetCounter(AallahiminsallahGameModeBase::ChartExample->GetCounter() + DeltaTime);
	FakeCounter = AallahiminsallahGameModeBase::ChartExample->GetCounter();
	FakeCounter = ((FakeCounter * 10) * 5) + 1480; //X
	FakeAltitude2 = 420 - ((FakeAltitude) / 2.0f); //Y

	FakeAltitude += FMath::RandRange(-5.0f, 7.0f);

	if (FakeAltitude >= 700)
	{
		fall = true;
	}

	if (fall)
	{
		FakeAltitude -= FMath::RandRange(0.0f, 15.0f);

	}

	if (FakeAltitude <= 10)
	{
		fall = false;
	}

	if (AallahiminsallahGameModeBase::ChartExample->LinesArray.Num() > 50)
	{
		FVector2D VectorPop = AallahiminsallahGameModeBase::ChartExample->LinesArray.HeapTop();
		AallahiminsallahGameModeBase::ChartExample->LinesArray.RemoveAt(0);
		float XEffect = VectorPop.X - 1480;
		float YEffect = 420 - VectorPop.Y;

		for (int64 Index = 0; Index != AallahiminsallahGameModeBase::ChartExample->LinesArray.Num(); ++Index)
		{
			FVector2D Vector = AallahiminsallahGameModeBase::ChartExample->LinesArray[Index];
			AallahiminsallahGameModeBase::ChartExample->LinesArray[Index] = FVector2D(Vector.X - XEffect, Vector.Y + YEffect);
		}
		UniversalXEffect += XEffect;
		UniversalYEffect += YEffect;
	}

	AallahiminsallahGameModeBase::ChartExample->SetLeftStickValue(FVector2D(FakeAltitude, FakeAltitude2 + UniversalYEffect));
	AallahiminsallahGameModeBase::ChartExample->LinesArray.Add(FVector2D(FakeCounter - UniversalXEffect,
		FakeAltitude2 + UniversalYEffect));
	*/

}



