// Fill out your copyright notice in the Description page of Project Settings.

#include "RoketDenemesi1.h"

MapSystem* MSystem;
UParticleSystemComponent* FireEffect;
UParticleSystemComponent* Phase1Smoke1;
UParticleSystemComponent* Phase1Smoke2;
UParticleSystemComponent* Phase2Smoke1;
UParticleSystemComponent* Phase2Smoke2;
USceneComponent* Phase1Scene;
USceneComponent* Phase2Scene;
APlayerController* PlayerController;
UStaticMeshComponent* CubePhysics;
UStaticMeshComponent* OffMainParachute;
UStaticMeshComponent* OffPayloadParachute;
UStaticMeshComponent* OffScudParachute;
UStaticMeshComponent* OnMainParachute;
UStaticMeshComponent* OnPayloadParachute;
UStaticMeshComponent* OnScudParachute;

ARoketDenemesi1::ARoketDenemesi1()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	PrimaryActorTick.bCanEverTick = false;
}
void ARoketDenemesi1::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController->GetViewTarget() != MainCamera)
	{
		PlayerController->SetViewTarget(MainCamera);
	}
	for (UActorComponent* Scene : GetComponentsByClass(USceneComponent::StaticClass()))
	{
		if (Scene->GetFName().IsEqual("Phase1")) Phase1Scene = Cast<USceneComponent>(Scene);
		else if (Scene->GetFName().IsEqual("Phase2")) Phase2Scene = Cast<USceneComponent>(Scene);
	}

	for (UActorComponent* Particle : GetComponentsByClass(UParticleSystemComponent::StaticClass()))
	{
		if (Particle->GetFName().IsEqual("fire_effect"))  FireEffect = Cast<UParticleSystemComponent>(Particle);
		else if (Particle->GetFName().IsEqual("phase1_smoke_1")) Phase1Smoke1 = Cast<UParticleSystemComponent>(Particle);
		else if (Particle->GetFName().IsEqual("phase1_smoke_2")) Phase1Smoke2 = Cast<UParticleSystemComponent>(Particle);
		else if (Particle->GetFName().IsEqual("phase2_smoke_1")) Phase2Smoke1 = Cast<UParticleSystemComponent>(Particle);
		else if (Particle->GetFName().IsEqual("phase2_smoke_2")) Phase2Smoke2 = Cast<UParticleSystemComponent>(Particle);

	}

	for (UActorComponent* StaticMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
	{
		if (StaticMesh->GetFName().IsEqual("CubePhysics")) CubePhysics = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("Roket2022_Faydali_yuk_parasutu_paket2")) OffPayloadParachute = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("Roket2022_Ana_parasut_paket")) OffMainParachute = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("Roket2022_Suruklenme_Parasutu_paket3")) OffScudParachute = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("Phase1Parachute")) OnScudParachute = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("Phase2Parachute")) OnMainParachute = Cast<UStaticMeshComponent>(StaticMesh);
		else if (StaticMesh->GetFName().IsEqual("PayloadParachute")) OnPayloadParachute = Cast<UStaticMeshComponent>(StaticMesh);
	}
	FireEffect->SetVisibility(true);
	FPortObserver::Attach(this);
	MSystem = new MapSystem();
}

bool GpsSecurity;
float PrevLat;
float PrevLon;
float SpeedRocket = 0;
float PrevSpeedRocket = 0;
float XSpeed = 0;
float YSpeed = 0;
float XMeter = 0;
float YMeter = 0;
bool Phase1Completed;
bool Phase2Completed;
FTimerHandle CountdownTimerHandle;
void ARoketDenemesi1::Update(RocketData Data, EListenerType AvionicType, const float DTime)
{
	if (AvionicType != MainAvionic) return;
	if (DTime == -1) return;

	if (Data.MainData.altitude >= 3000) FireEffect->SetVisibility(false);
	if (Data.MainData.altitude <= 0) FireEffect->SetVisibility(true);
	if (Data.MainData.altitude >= 200)
	{
		if (PlayerController->GetViewTarget() != RCamera)
		{
			PlayerController->SetViewTarget(RCamera);
		}
	}
	else
	{
		if (PlayerController->GetViewTarget() != MainCamera)
		{
			PlayerController->SetViewTarget(MainCamera);
		}
	}

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	
	FVector RocketLocation = GetActorLocation();
	RocketLocation.Set(GetActorLocation().X, GetActorLocation().Y, Data.MainData.altitude * 20);
	UKismetSystemLibrary::MoveComponentTo(RootComponent, RocketLocation, 
		FRotator(0, 0, 0)
		, false, false, DTime + 0.4, false, EMoveComponentAction::Type::Move, LatentInfo);

	if (Phase1Completed) Phase1Scene->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y + 300, GetActorLocation().Z));
	if (!Phase1Completed && Data.MainData.rocketState == 2)
	{
		CubePhysics->SetSimulatePhysics(true);
		Phase1Smoke1->SetVisibility(true);
		Phase1Smoke2->SetVisibility(true);
		for (uint8_t i = 0; i < Phase1Scene->GetNumChildrenComponents(); i++)
		{
			if (Phase1Scene->GetChildComponent(i)->GetFName().IsEqual("Rope2")) Phase1Scene->GetChildComponent(i)->SetVisibility(true);
		}
		Phase1Scene->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y + 300, GetActorLocation().Z));
		UKismetSystemLibrary::MoveComponentTo(RootComponent, RocketLocation,
			FRotator(0, 0, 90)
			, false, false, DTime + 5, false, EMoveComponentAction::Type::Move, LatentInfo);

		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ARoketDenemesi1::StopPhase1Smoke, 0.4f, true);
		Phase1Completed = true;
	}


	if (Phase2Completed) Phase2Scene->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y - 300, GetActorLocation().Z));
	if (!Phase2Completed && Data.MainData.rocketState == 4)
	{
		Phase2Smoke1->SetVisibility(true);
		Phase2Smoke2->SetVisibility(true);
		for (uint8_t i = 0; i < Phase2Scene->GetNumChildrenComponents(); i++)
		{
			if (Phase2Scene->GetChildComponent(i)->GetFName().IsEqual("Rope3")) Phase2Scene->GetChildComponent(i)->SetVisibility(true);
		}
		Phase2Scene->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y - 300, GetActorLocation().Z));
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ARoketDenemesi1::StopPhase2Smoke, 0.4f, true);
		Phase2Completed = true;
	}
}

void ARoketDenemesi1::StopPhase1Smoke()
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	OffScudParachute->SetVisibility(false);
	OffPayloadParachute->SetVisibility(false);
	OnScudParachute->SetVisibility(true);
	OnPayloadParachute->SetVisibility(true);
	Phase1Smoke1->SetVisibility(false);
	Phase1Smoke2->SetVisibility(false);
	Phase2Smoke1->SetVisibility(false);
	Phase2Smoke2->SetVisibility(false);
	CubePhysics->SetSimulatePhysics(false);
}

void ARoketDenemesi1::StopPhase2Smoke()
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	OffMainParachute->SetVisibility(false);
	OnMainParachute->SetVisibility(true);
	Phase2Smoke1->SetVisibility(false);
	Phase2Smoke2->SetVisibility(false);
}

void ARoketDenemesi1::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FPortObserver::Detach(this);
	FPortObserver::FirstData = false;
}

