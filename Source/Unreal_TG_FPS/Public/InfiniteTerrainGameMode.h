// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unreal_TG_FPSGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_TG_FPS_API AInfiniteTerrainGameMode : public AUnreal_TG_FPSGameMode
{
	GENERATED_BODY()

	AInfiniteTerrainGameMode();

	void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);

	int CurrentScore = 0;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsVolumePool;

public:
	UFUNCTION(BlueprintCallable, Category = "Nav Mesh")
	void PopulateBoundsVolumePool();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void IncrementScore();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int GetScore() const;
};
