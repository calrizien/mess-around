// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

class UBaseGameplayAbility;

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	public:
	// Constructors and overrides
	UBaseAbilitySystemComponent();

	// Have the owning Character's Default Abilities been given yet?
	bool DefaultAbilitiesGiven = false;

	// Have the owning Character's Default Effects been applied yet?
	bool DefaultEffectsApplied = false;
	
	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UBaseGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UBaseAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
};
