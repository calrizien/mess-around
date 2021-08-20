// Copyright Brandon Winston 2021


#include "NonPlayerCharacter.h"
#include "BaseAbilitySystemComponent.h"
#include "BaseAttributeSet.h"

ANonPlayerCharacter::ANonPlayerCharacter()
{
	
	// Create the Non Player Character Ability System Component sub-object.
	NPC_AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(FName("Ability System Component"));

	// Explicitly set the Ability System Component to replicate.
	NPC_AbilitySystemComponent->SetIsReplicated(true);

	// Sets the Ability System Component to use Minimal replication mode. This will only replicate minimal Gameplay Effect info to connected clients.
	NPC_AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Minimal;
	
	// Set the TWeakObjectPtr from Character Base to the newly created Ability System Component sub-object.
	AbilitySystemComponent = NPC_AbilitySystemComponent;
	// TODO: Create an NPC Attribute set with Aggro, hate list, (maybe something for faction or to link with a faction system?) and set it here
	// Create the Attribute Set sub-object.
	NPC_Attributes = CreateDefaultSubobject<UBaseAttributeSet>(FName("NPC Attributes"));

	// Set the TWeakObjectPtr from Character Base to the newly created Attribute Set sub-object.
	AttributeSet = NPC_Attributes;
	

}

void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent.IsValid())
	{
		// Set the Owner and Avatar actor for the Ability System Component.
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		GiveDefaultAbilities();

		ApplyDefaultEffects();
	}
}
