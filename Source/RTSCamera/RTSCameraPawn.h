// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSCameraPawn.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraComponent;

UCLASS()
class RTSCAMERA_API ARTSCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCameraPawn();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue& InValue);
	void MoveCompleted();
	void ZoomTriggered(const FInputActionValue& InValue);
	void RotationStarted();
	void RotationCompleted();
	void LookTriggered(const FInputActionValue& InValue);

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* ZoomAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ScreenEdgePadding;

	UPROPERTY(EditDefaultsOnly)
	FVector2D MoveSpeed;

	UPROPERTY(EditDefaultsOnly)
	float CameraHeightSpeed;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed;

	FVector2D CurrentInputMoveSpeed;
	bool ShouldRotate;

	UPROPERTY()
	class UCameraComponent* CameraComponent;
};
