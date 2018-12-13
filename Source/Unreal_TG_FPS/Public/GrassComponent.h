// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_TG_FPS_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int SpawnCount = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		FBox SpawningBounds;

	void SpawnGrass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
