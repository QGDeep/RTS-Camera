// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSCameraPawn.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
//#include "../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
//#include "../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
//#include "../../../../../../../Program Files/Epic Games/UE_5.4/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

// Sets default values
ARTSCameraPawn::ARTSCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera Setup
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);

	MoveSpeed = FVector2D(500, 500);
	ScreenEdgePadding = FVector2D(60, 60);
	CameraHeightSpeed = 200.0;
	ShouldRotate = false;
	RotationSpeed = 50.0;

	SetActorRotation(FRotator::MakeFromEuler(FVector3d(0, -30, 0)));
}

// Called when the game starts or when spawned
void ARTSCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController = Cast<APlayerController>(Controller); IsValid(PlayerController)) {
		PlayerController->SetShowMouseCursor(true);
	}
}

// Called every frame
void ARTSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate the correct move speed of the camera
	auto CurrentMoveSpeed = CurrentInputMoveSpeed;
	if (FVector2D MousePosition; CurrentInputMoveSpeed.X == 0 && CurrentInputMoveSpeed.Y == 0 &&
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y)) {

		int32 ViewportWidth, ViewportHeight;

		PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

		const auto X = -(MousePosition.X <= ScreenEdgePadding.X) | (MousePosition.X >= ViewportWidth - ScreenEdgePadding.X);

		const auto Y = (MousePosition.Y <= ScreenEdgePadding.Y) | -(MousePosition.Y >= ViewportHeight - ScreenEdgePadding.Y);

		CurrentMoveSpeed = FVector2D(X, Y);
	}

	// Move the camera
	auto ForwardVector = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0);
	ForwardVector.Normalize();

	const auto Forward = ForwardVector * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;
	const auto Sideways = GetActorRightVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;

	const auto NextLocation = GetActorLocation() + Forward + Sideways;
	SetActorLocation(NextLocation);
}

// Called to bind functionality to input
void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup Inputsystem
	// Casting to player controller during setup to avoid crashing during
	// runtime (Beginplay was executed after this setup functions resulting in nullptr)
	if (const auto Subsystem = Cast<APlayerController>(GetController())->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {

		Subsystem->AddMappingContext(MappingContext, 0);
	}

	// Bind actions
	if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARTSCameraPawn::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARTSCameraPawn::MoveCompleted);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARTSCameraPawn::ZoomTriggered);

		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Started, this, &ARTSCameraPawn::RotationStarted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Canceled, this, &ARTSCameraPawn::RotationCompleted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &ARTSCameraPawn::RotationCompleted);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARTSCameraPawn::LookTriggered);

	}
}

void ARTSCameraPawn::MoveTriggered(const FInputActionValue& InValue)
{
	CurrentInputMoveSpeed = InValue.Get<FVector2D>();
}

void ARTSCameraPawn::MoveCompleted()
{
	CurrentInputMoveSpeed = FVector2D::Zero();
}

void ARTSCameraPawn::ZoomTriggered(const FInputActionValue& InValue)
{
	auto Location = GetActorLocation();
	Location.Z += InValue.Get<float>() * CameraHeightSpeed * FApp::GetDeltaTime();
	SetActorLocation(Location);
}

void ARTSCameraPawn::RotationStarted()
{
	ShouldRotate = true;
}

void ARTSCameraPawn::RotationCompleted()
{
	ShouldRotate = false;
}

void ARTSCameraPawn::LookTriggered(const FInputActionValue& InValue)
{
	const auto& LookVector = InValue.Get<FVector2D>();

	if (ShouldRotate) {
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Pitch += RotationSpeed * FApp::GetDeltaTime() * LookVector.Y;
		CurrentRotation.Yaw += RotationSpeed * FApp::GetDeltaTime() * LookVector.X;
		SetActorRotation(CurrentRotation);
	}
}

