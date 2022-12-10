// Fill out your copyright notice in the Description page of Project Settings.


#include "SerialPortWidget.h"

#include "Components/TextBlock.h"


void USerialPortWidget::NativeConstruct()
{
    FPortObserver::Attach(this);

    RefreshButton->OnClicked.AddUniqueDynamic(this, &USerialPortWidget::RefreshCom);
    ConnectButtonMain->OnClicked.AddUniqueDynamic(this, &USerialPortWidget::ToggleMainPort);
    ConnectButtonPayload->OnClicked.AddUniqueDynamic(this, &USerialPortWidget::TogglePayloadPort);
    ConnectButtonRefereeStation->OnClicked.AddUniqueDynamic(this, &USerialPortWidget::ToggleRefereeStationPort);
    RefreshCom();
}

void USerialPortWidget::NativeDestruct()
{
    FPortObserver::Detach(this);
}

void USerialPortWidget::Update(const RocketData NowData, EListenerType AvionicType, const float DTime)
{
	if (AvionicType == MainAvionic)
	{
        altitude->SetText(FText::FromString(FString("Irtifa: ").Append(FString::SanitizeFloat(NowData.MainData.altitude))));
        roll->SetText(FText::FromString(FString("Roll: ").Append(FString::SanitizeFloat(NowData.MainData.x))));
        yaw->SetText(FText::FromString(FString("Yaw: ").Append(FString::SanitizeFloat(NowData.MainData.z))));
        pitch->SetText(FText::FromString(FString("Pitch: ").Append(FString::SanitizeFloat(NowData.MainData.y))));
        latitude->SetText(FText::FromString(FString("Latitude: ").Append(FString::SanitizeFloat(NowData.MainData.latitude))));
        longitude->SetText(FText::FromString(FString("Longitude: ").Append(FString::SanitizeFloat(NowData.MainData.longitude))));
        gX ->SetText(FText::FromString(FString("Ivme X: ").Append(FString::SanitizeFloat(NowData.MainData.accX))));
        gY->SetText(FText::FromString(FString("Ivme Y: ").Append(FString::SanitizeFloat(NowData.MainData.accY))));
        gZ->SetText(FText::FromString(FString("Ivme Z: ").Append(FString::SanitizeFloat(NowData.MainData.accZ))));

	}else if(AvionicType == PayloadAvionic)
	{
        temperature->SetText(FText::FromString(FString("Sicaklik: ").Append(FString::SanitizeFloat(NowData.PayloadData.temperature))));
        altitude2->SetText(FText::FromString(FString("Irtifa: ").Append(FString::SanitizeFloat(NowData.PayloadData.altitude))));
        humidity->SetText(FText::FromString(FString("Nem: ").Append(FString::SanitizeFloat(NowData.PayloadData.humidity))));
        latitude2->SetText(FText::FromString(FString("Enlem: ").Append(FString::SanitizeFloat(NowData.PayloadData.latitude))));
        longitude2->SetText(FText::FromString(FString("Boylam: ").Append(FString::SanitizeFloat(NowData.PayloadData.longitude))));
	}
}



void USerialPortWidget::RefreshCom()
{
    MainAvionicCombo->ClearOptions();
    PayloadAvionicCombo->ClearOptions();
    RefereeStation->ClearOptions();
    wchar_t lpTargetPath[5000]; // buffer to store the path of the COM PORTS

    for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
    {
        wstring str = L"COM" + to_wstring(i); // converting to COM0, COM1, COM2
        DWORD res = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

        if (res != 0)
        {
            MainAvionicCombo->AddOption(FString(str.c_str()));
            PayloadAvionicCombo->AddOption(FString(str.c_str()));
            RefereeStation->AddOption(FString(str.c_str()));
        }
        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
        }
    }
}

void USerialPortWidget::ToggleMainPort()
{
	if (MainAvionicCombo->GetSelectedOption().Equals("")) AvionicText->SetText(FText::FromString(FString("COM Secin")));
	else if (!MainSerial)
	{
        MainSerial = new SerialPort(MainAvionic, TCHAR_TO_ANSI(*MainAvionicCombo->GetSelectedOption()), CBR_57600);
        if (MainSerial->GetConnected()) AvionicText->SetText(FText::FromString(FString("Sonlandir")));
	}else
	{
        MainSerial->~SerialPort();
        AvionicText->SetText(FText::FromString(FString("Baglan")));
        MainSerial = nullptr;

	}
}

void USerialPortWidget::TogglePayloadPort()
{
    if (PayloadAvionicCombo->GetSelectedOption().Equals("")) PayloadText->SetText(FText::FromString(FString("COM Secin")));
    else if (!PayloadSerial)
    {
        PayloadSerial = new SerialPort(PayloadAvionic, TCHAR_TO_ANSI(*PayloadAvionicCombo->GetSelectedOption()), CBR_57600);
        if (PayloadSerial->GetConnected()) PayloadText->SetText(FText::FromString(FString("Sonlandir")));
    }
    else
    {
        PayloadSerial->~SerialPort();
        PayloadText->SetText(FText::FromString(FString("Baglan")));
        PayloadSerial = nullptr;

    }
}

void USerialPortWidget::ToggleRefereeStationPort()
{
    if (RefereeStation->GetSelectedOption().Equals("")) RefereeStationText->SetText(FText::FromString(FString("COM Secin")));
    else if (!RefereeStationSerial)
    {
        RefereeStationSerial = new SerialPort(EListenerType::RefereeStation, "\\\\.\\COM17", CBR_19200);
        if (RefereeStationSerial->GetConnected()) RefereeStationText->SetText(FText::FromString(FString("Sonlandir")));
    }
    else
    {
        RefereeStationSerial->~SerialPort();
        RefereeStationText->SetText(FText::FromString(FString("Baglan")));
        RefereeStationSerial = nullptr;

    }
}
