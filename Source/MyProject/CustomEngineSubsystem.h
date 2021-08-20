// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "CustomEngineSubsystem.generated.h"

/**
 * Initializes the UAbilitySystemGlobals module. 
 */
UCLASS()
class MYPROJECT_API UCustomEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
	public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;	
};
