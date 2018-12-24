// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavigationSystem.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavMeshBoundsVolOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting Pool: %s"), *(InPool->GetName()));
	Pool = InPool;
	
	PositionNavMeshBoundsVol();
}

void ATile::PositionNavMeshBoundsVol()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No actors to checkout of pool"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: %s"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshBoundsVolOffset);
	UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Pool && NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("EndPlay called on: %s"), *GetName());
		Pool->Return(NavMeshBoundsVolume);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> SpawnedActorType, int MinNumber, int MaxNumber, int CollisionCheckRadius, float MinScaleFactor, float MaxScaleFactor)
{
	TArray<FSpawnDetails> ArrayOfSpawnDetails;
	int randomNumOfActors = FMath::RandRange(MinNumber, MaxNumber);

	for (int i = 0; i < randomNumOfActors; i++)
	{
		FSpawnDetails SpawnDetails;
		SpawnDetails.ScaleFactor = FMath::RandRange(MinScaleFactor, MaxScaleFactor);

		bool HaveFoundEmptyLocation = FindEmptyLocation(SpawnDetails.Location, CollisionCheckRadius * SpawnDetails.ScaleFactor);

		if (HaveFoundEmptyLocation) {
			SpawnDetails.YawRotation = FMath::RandRange(-180.f, 180.f);
			PlaceSingleActor(SpawnedActorType, SpawnDetails);
		}
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> SpawnedActorType, int MinNumber, int MaxNumber, int CollisionCheckRadius, float MinScaleFactor, float MaxScaleFactor) 
{
	RandomlyPlaceActors(SpawnedActorType, MinNumber, MaxNumber, CollisionCheckRadius, MinScaleFactor, MaxScaleFactor);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> SpawnedPawnType, int MinNumber, int MaxNumber, int CollisionCheckRadius)
{
	RandomlyPlaceActors(SpawnedPawnType, MinNumber, MaxNumber, CollisionCheckRadius, 1, 1);
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

void ATile::PlaceSingleActor(TSubclassOf<AActor> SpawnedActorType, const FSpawnDetails& SpawnDetails) 
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedActorType);
	if (SpawnedActor) {
		SpawnedActor->SetActorRelativeLocation(SpawnDetails.Location);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedActor->SetActorRotation(FRotator(0, SpawnDetails.YawRotation, 0));
		SpawnedActor->SetActorRelativeScale3D(FVector(SpawnDetails.ScaleFactor));
	}
}


void ATile::PlaceSingleActor(TSubclassOf<APawn> SpawnedPawnType, const FSpawnDetails& SpawnDetails)
{
	FRotator Rotation = FRotator(0, SpawnDetails.YawRotation, 0);
	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(SpawnedPawnType, SpawnDetails.Location, Rotation);
	if (SpawnedPawn) {
		SpawnedPawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		SpawnedPawn->SpawnDefaultController();
		SpawnedPawn->Tags.Add(FName("Mates"));
	}
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

