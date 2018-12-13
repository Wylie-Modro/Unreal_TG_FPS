// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATile::PlaceActors(TSubclassOf<AActor> SpawnedActorType, int MinNumber, int MaxNumber, int CollisionCheckRadius, float MinScaleFactor, float MaxScaleFactor) 
{
	int randomNumOfActors = FMath::RandRange(MinNumber, MaxNumber);

	for (int i = 0; i < randomNumOfActors; i++)
	{
		FVector SpawnPoint;
		float ScaleFactor = FMath::RandRange(MinScaleFactor, MaxScaleFactor);

		bool HaveFoundEmptyLocation = FindEmptyLocation(SpawnPoint, CollisionCheckRadius * ScaleFactor);

		if (HaveFoundEmptyLocation) {
			float RandomYawRotation = FMath::RandRange(-180.f, 180.f);
			PlaceSingleActor(SpawnedActorType, SpawnPoint, RandomYawRotation, ScaleFactor);
		}

	}
}

bool ATile::FindEmptyLocation(FVector& EmptyLocation, float Radius) {

	FBox BoxBounds = FBox(FVector(0.0f, -2000.0f, 0.0f), FVector(4000.0f, 2000.0f, 0.0f));
	const int MAX_ATTEMPTS = 100;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidateLocation = FMath::RandPointInBox(BoxBounds);
		if (CanSpawnAtLocation(CandidateLocation, Radius)) {
			EmptyLocation = CandidateLocation;
			return true;
		}
	}

	return false;
}

void ATile::PlaceSingleActor(TSubclassOf<AActor> SpawnedActorType, FVector SpawnLocation, float SpawnYawRotation, float ScaleFactor) {

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedActorType);
	SpawnedActor->SetActorRelativeLocation(SpawnLocation);
	SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	SpawnedActor->SetActorRotation(FRotator(0, SpawnYawRotation, 0));
	SpawnedActor->SetActorRelativeScale3D(FVector(ScaleFactor));
}

bool ATile::CanSpawnAtLocation(FVector CandidateLocation, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(CandidateLocation);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	/* DrawDebug Spheres to check collisions
	if (HasHit) {
		DrawDebugSphere(GetWorld(), GlobalLocation, Radius, 20, FColor::Red, true, 1000);
	}
	else {
		DrawDebugSphere(GetWorld(), GlobalLocation, Radius, 20, FColor::Green, true, 1000);
	}
	*/

	return !HasHit;
}

