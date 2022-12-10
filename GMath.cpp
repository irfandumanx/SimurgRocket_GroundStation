// Fill out your copyright notice in the Description page of Project Settings.


#include "GMath.h"

GMath::GMath()
{
}

GMath::~GMath()
{
}

std::pair<float, float> GMath::PredictGpsData(float AzimuthAngle, float Speed, float PrevLat, float PrevLon, float DTime)
{

	const float X = Speed * sin(AzimuthAngle * PI / 180) * DTime;
	const float Y = Speed * cos(AzimuthAngle * PI / 180) * DTime;

	float Lat = PrevLat + (180 / PI) * (Y / WORLD_RADIUS);
	float Lon = PrevLon + (180 / PI) / sin(PrevLat * PI / 180) * (X / WORLD_RADIUS);

	return std::pair(Lat, Lon);
}
