// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class UNREAL_TG_FPS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		void PlaceActors(TSubclassOf<AActor> SpawnedActorType, int MinNumber = 1, int MaxNumber = 4, int Radius = 300, float MinScaleFactor = 0.5f, float MaxScaleFactor = 2.f);

	void PlaceSingleActor(TSubclassOf<AActor> SpawnedActorType, FVector SpawnLocation, float SpawnYawRotation, float ScaleFactor);

	bool CanSpawnAtLocation(FVector CandidiateLocation, float Radius);

	bool FindEmptyLocation(FVector& EmptyLocation, float Radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
