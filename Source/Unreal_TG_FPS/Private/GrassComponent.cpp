// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassComponent.h"

void UGrassComponent::BeginPlay() 
{
	Super::BeginPlay();
	SpawnGrass();
}

void UGrassComponent::SpawnGrass() 
{
	for (int i = 0; i < SpawnCount; i++)
	{
		FVector SpawnLocation = FMath::RandPointInBox(SpawningBounds);
		AddInstance(FTransform(SpawnLocation));
	}
}
