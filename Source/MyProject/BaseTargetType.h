// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BaseAbilityTypes.h"
#include "BaseTargetType.generated.h"


class ABaseCharacter;
class AActor;
struct FGameplayEventData;

/**
* Class that is used to determine targeting for abilities
* It is meant to be blueprinted to run target logic
* This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
* This can be used as a basis for a game-specific targeting blueprint
* If your targeting is more complicated you may need to instance into the world once or as a pooled actor
*/

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBaseTargetType : public UObject
{
	GENERATED_BODY()

	public:
	// Constructor and overrides
	UBaseTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ABaseCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class MYPROJECT_API UBaseTargetType_UseOwner : public UBaseTargetType
{
	GENERATED_BODY()

	public:
	// Constructor and overrides
	UBaseTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ABaseCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class MYPROJECT_API UBaseTargetType_UseEventData : public UBaseTargetType
{
	GENERATED_BODY()

	public:
	// Constructor and overrides
	UBaseTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ABaseCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
