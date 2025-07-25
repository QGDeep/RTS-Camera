// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSCameraPawn.h"

ARTSGameMode::ARTSGameMode() {

	DefaultPawnClass = ARTSCameraPawn::StaticClass();

	static ConstructorHelpers::FClassFinder<ARTSCameraPawn> PlayerPawnBPClass(TEXT("/Content/Blueprints/BP_RTSCameraPawn"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}