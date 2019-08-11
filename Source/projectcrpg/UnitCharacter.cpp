// Copyright Randall Hines 2019


#include "UnitCharacter.h"
#include "UnitAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();
	unitAI = Cast<AUnitAIController>(GetController());
}

// Called every frame
void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (true) {
	//	FVector dir = (destination - GetActorTransform().GetLocation());
	//	dir.Normalize();
	//	GetMovementComponent()->AddInputVector(dir);
	//}
}

// Called to bind functionality to input
void AUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnitCharacter::Move(FVector pos) {
	if (unitAI != nullptr) {
		unitAI->GoToDestination(pos);
	}
}

