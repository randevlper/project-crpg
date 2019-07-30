// Copyright Randall Hines 2019


#include "PlayerUnitControllerPawn.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "UnitCharacter.h"
#include "UnitPlayerController.h"
#include "DrawDebugHelpers.h"

//For Debugging
#include "Engine/Engine.h"

// Sets default values
APlayerUnitControllerPawn::APlayerUnitControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 0;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	FRotator cameraRot(-45.0f,0,0);
	Camera->SetRelativeRotation(cameraRot);
	controlledCharacter = nullptr;
}

// Called when the game starts or when spawned
void APlayerUnitControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController<AUnitPlayerController>();
}

// Called every frame
void APlayerUnitControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerUnitControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerUnitControllerPawn::InputHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerUnitControllerPawn::InputVertical);
	
	PlayerInputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &APlayerUnitControllerPawn::Exit);
	PlayerInputComponent->BindAction("RightClick", EInputEvent::IE_Pressed, this, &APlayerUnitControllerPawn::CheckForCharacter);
	PlayerInputComponent->BindAction("LeftClick", EInputEvent::IE_Pressed, this, &APlayerUnitControllerPawn::CheckForDestination);
}

void APlayerUnitControllerPawn::InputHorizontal(float value) {
	if ((Controller != NULL) && (value != 0.0f)) {
		const FVector direction = GetActorRightVector();
		AddMovementInput(direction, value);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, "Horizontal: " + FString::SanitizeFloat(value));
		}
	}
}

void APlayerUnitControllerPawn::InputVertical(float value) {

	if ((Controller != NULL) && (value != 0.0f)) {
		const FVector direction = GetActorForwardVector();
		AddMovementInput(direction, value);

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, "Vertical: " + FString::SanitizeFloat(value));
		}
	}
}

void APlayerUnitControllerPawn::Exit() {
	FGenericPlatformMisc::RequestExit(false);
}

void APlayerUnitControllerPawn::CheckForCharacter() {
	FVector mousePosition, mouseDirection;
	if (playerController->DeprojectMousePositionToWorld(mousePosition, mouseDirection)){
		FHitResult hit;
		
		UWorld* world = GetWorld();

		if (world != NULL) {
			DrawDebugLine(world, mousePosition, mousePosition + (mouseDirection * 1000), FColor::Red, false, 1.0f);
			if (world->LineTraceSingleByChannel(hit, mousePosition, (mousePosition + mouseDirection * 1000),
				ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams::DefaultQueryParam,
				FCollisionResponseParams::DefaultResponseParam)) {
				//controlledCharacter->Move(hit.Location);
				if (hit.GetActor() != NULL) {
					AUnitCharacter* character = Cast<AUnitCharacter>(hit.GetActor());
					if (character) {
						DrawDebugPoint(world, hit.Location, 100.0f, FColor::Blue);
						controlledCharacter = character;
					}
					else {
						controlledCharacter = nullptr;
					}
				}
			}
		}
	}
}

void APlayerUnitControllerPawn::CheckForDestination() {
	FVector mousePosition, mouseDirection;
	if (playerController->DeprojectMousePositionToWorld(mousePosition, mouseDirection)) {
		FHitResult hit;
		UWorld* world = GetWorld();

		if (world != NULL) {
			DrawDebugLine(world, mousePosition, mousePosition + (mouseDirection * 1000), FColor::Red, false, 1.0f);
			if (world->LineTraceSingleByChannel(hit, mousePosition, (mousePosition + mouseDirection * 1000),
				ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams::DefaultQueryParam,
				FCollisionResponseParams::DefaultResponseParam)) {
				if (controlledCharacter != nullptr) {
					controlledCharacter->Move(hit.Location);
				}
			}
		}
	}
}

