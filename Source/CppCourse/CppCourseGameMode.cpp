// Copyright Epic Games, Inc. All Rights Reserved.

#include "CppCourseGameMode.h"
#include "CppCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACppCourseGameMode::ACppCourseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
