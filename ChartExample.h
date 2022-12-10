// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SerialPort.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ChartExample.generated.h"

/**
 * 
 */
class UWrapBox;

UCLASS()
class ALLAHIMINSALLAH_API UChartExample : public UUserWidget, public FPortListener
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		TArray<int32> LeftStickValues;
	UFUNCTION(BlueprintGetter)
		TArray<int32> GetLeftStickValues() { return LeftStickValues; }
	UFUNCTION(BlueprintSetter)
		void SetLeftStickValues(TArray<int32> NewArray) { LeftStickValues = NewArray; }

	UPROPERTY(BlueprintReadWrite)
		TArray<int32> UnderStickValues;
	UFUNCTION(BlueprintGetter)
		TArray<int32> GetUnderStickValues() { return UnderStickValues; }
	UFUNCTION(BlueprintSetter)
		void SetUnderStickValues(TArray<int32> NewArray) { UnderStickValues = NewArray; }

	UPROPERTY(BlueprintReadWrite)
		TArray<FVector2D> LinesArray;
	UFUNCTION(BlueprintGetter)
		TArray<FVector2D> GetLinesArray() { return LinesArray; }
	UFUNCTION(BlueprintSetter)
		void SetLinesArray(TArray<FVector2D> NewValues) {}

	UPROPERTY(BlueprintReadWrite)
		float Counter;
	UFUNCTION(BlueprintGetter, BlueprintCallable)
		float GetCounter() { return Counter; }
	UFUNCTION(BlueprintSetter)
		void SetCounter(float Counter_) { Counter = Counter_; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void Update(const RocketData NowData, EListenerType AvionicType, const float DTime) override;
};
