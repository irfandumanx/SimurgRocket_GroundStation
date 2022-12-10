// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "allahiminsallahGameModeBase.h"
#include "SerialPortWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALLAHIMINSALLAH_API USerialPortWidget : public UUserWidget, public FPortListener
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* MainAvionicCombo;
	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* PayloadAvionicCombo;
	UPROPERTY(meta = (BindWidget))
		class UComboBoxString* RefereeStation;
	UPROPERTY(meta = (BindWidget))
		class UButton* RefreshButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConnectButtonMain;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConnectButtonPayload;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConnectButtonRefereeStation;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvionicText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PayloadText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RefereeStationText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* altitude;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* roll;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* yaw;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* pitch;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* latitude;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* longitude;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* gX;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* gY;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* gZ;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* temperature;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* altitude2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* humidity;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* latitude2;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* longitude2;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void Update(const RocketData NowData, EListenerType AvionicType, const float DTime) override;
	UFUNCTION()
		void RefreshCom();
	UFUNCTION()
		void ToggleMainPort();
	UFUNCTION()
		void TogglePayloadPort();
	UFUNCTION()
		void ToggleRefereeStationPort();
private:
	SerialPort* MainSerial;
	SerialPort* PayloadSerial;
	SerialPort* RefereeStationSerial;
};
