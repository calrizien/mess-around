// Copyright Brandon Winston 2021


#include "BaseAbilitySystemComponent.h"

#include "BaseCharacter.h"
#include "MyAbilitySystemGlobals.h"
#include "BaseGameplayAbility.h"

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent() {}

void UBaseAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UBaseGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UBaseGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UBaseAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ABaseCharacter* OwningCharacter = Cast<ABaseCharacter>(GetOwnerActor());

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

UBaseAbilitySystemComponent* UBaseAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UBaseAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}