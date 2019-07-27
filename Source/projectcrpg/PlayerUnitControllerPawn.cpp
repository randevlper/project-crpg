// Copyright Randall Hines 2019


#include "PlayerUnitControllerPawn.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

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
}

// Called when the game starts or when spawned
void APlayerUnitControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	
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

