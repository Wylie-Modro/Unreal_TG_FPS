// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnDetails
{
	GENERATED_USTRUCT_BODY();

	FVector Location;
	float YawRotation;
	float ScaleFactor;
};

class UActorPool;
class AInfiniteTerrainGameMode;

UCLASS()
class UNREAL_TG_FPS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceActors(TSubclassOf<AActor> SpawnedActorType, int MinNumber = 1, int MaxNumber = 4, int CollisionCheckRadius = 300, float MinScaleFactor = 0.5f, float MaxScaleFactor = 2.f);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(TSubclassOf<APawn> SpawnedPawnType, int MinNumber = 1, int MaxNumber = 4, int CollisionCheckRadius = 300);

	void PlaceSingleActor(TSubclassOf<AActor> SpawnedActorType, const FSpawnDetails& SpawnDetails);
	void PlaceSingleActor(TSubclassOf<APawn> SpawnedPawnType, const FSpawnDetails& SpawnDetails);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> SpawnedActorType, int MinNumber = 1, int MaxNumber = 4, int CollisionCheckRadius = 300, float MinScaleFactor = 0.5f, float MaxScaleFactor = 2.f);

	bool CanSpawnAtLocation(FVector CandidiateLocation, float Radius);

	bool FindEmptyLocation(FVector& EmptyLocation, float Radius);

	UPROPERTY(EditDefaultsOnly, Category = "Nav")
	FVector NavMeshBoundsVolOffset;

	void PositionNavMeshBoundsVol();

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);
};
