// Copyright Randall Hines 2019


#include "UnitAIController.h"

void AUnitAIController::BeginPlay(){
	Super::BeginPlay();
}

void AUnitAIController::GoToDestination(FVector Destination) {
	MoveToLocation(Destination);
}