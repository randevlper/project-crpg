// Copyright Randall Hines 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCRPG_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void GoToDestination(FVector Destination);
};
