// Copyright Brandon Winston 2021


#include "CustomEngineSubsystem.h"
#include "MyAbilitySystemGlobals.h"

void UCustomEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();	
}
