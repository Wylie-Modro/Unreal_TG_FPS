// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "PatrollingRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {


	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	
	// Get Patrol Points
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	UPatrollingRoute* PatrollingRoute = ControlledPawn->FindComponentByClass<UPatrollingRoute>();
	if (!ensure(PatrollingRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrollingRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("This lad doesn't have any patrol points")); 
		return EBTNodeResult::Failed;
	}

	// Set Next Waypoint
	BlackboardComponent->SetValueAsObject(CurrentWaypointKey.SelectedKeyName, PatrolPoints[Index]);

	//Cycle Index
	auto NumOfPatrolPoints = PatrolPoints.Num();
	auto NextIndex = (++Index) % (NumOfPatrolPoints);
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
		
	return EBTNodeResult::Succeeded;
}

