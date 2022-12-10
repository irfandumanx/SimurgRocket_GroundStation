// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define WORLD_RADIUS 6378137.0

/**
 * 
 */
class ALLAHIMINSALLAH_API GMath
{
public:
	GMath();
	~GMath();
	static std::pair<float, float> PredictGpsData(float AzimuthAngle, float Speed, float PrevLat, float PrevLon, float DTime);
};
