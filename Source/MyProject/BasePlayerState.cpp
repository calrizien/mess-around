// Copyright Brandon Winston 2021


#include "BasePlayerState.h"
#include "BaseAttributeSet.h"

ABasePlayerState::ABasePlayerState()
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	NetUpdateFrequency = 100.0f;

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(FName("Ability System Component"));

	// Explicitly set the Ability System Component to replicate.
	AbilitySystemComponent->SetIsReplicated(true);

	// Sets the Ability System Component to use Full replication mode. This will replicate Gameplay Effects to everyone.
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Full;

	// Create the Health Attribute Set sub-object.
	Attributes = CreateDefaultSubobject<UBaseAttributeSet>(FName("Base Attributes"));
	
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

