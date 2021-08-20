// Copyright Brandon Winston 2021


#include "PlayerCharacter.h"

#include "BasePlayerState.h"
#include "BaseAbilitySystemComponent.h"
#include "GameplayAbilityBindings.h"
#include "BaseAttributeSet.h"

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!GetPlayerState() || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	ABasePlayerState* PlayerStateBase = Cast<ABasePlayerState>(GetPlayerState());

	if (PlayerStateBase)
	{
		AbilitySystemComponent = PlayerStateBase->AbilitySystemComponent;

		// Set the Owner and Avatar actor for the Ability System Component. (On the Server)
		AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);

		AttributeSet = PlayerStateBase->Attributes;



		GiveDefaultAbilities();

		ApplyDefaultEffects();
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABasePlayerState* PlayerStateBase = Cast<ABasePlayerState>(GetPlayerState());

	if (PlayerStateBase)
	{
		AbilitySystemComponent = PlayerStateBase->AbilitySystemComponent;

		// Set the Owner and Avatar actor for the Ability System Component. (On the Client)
		AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);

		AttributeSet = PlayerStateBase->Attributes;

		InitializeAbilityBindings();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializeAbilityBindings();
}

void APlayerCharacter::InitializeAbilityBindings()
{
	if (AbilityBindingsInitialized || !GetPlayerState() || !AbilitySystemComponent.Get() || !InputComponent)
	{
		return;
	}

	// Binds GA input to the Input component.
	// These input bindings can be edited in "GameplayAbilityBindings.h" (Do not remove "None", "Confirm", or "Cancel")
	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("EGameplayAbilityBindings"), static_cast<int32>(EGameplayAbilityBindings::Confirm), static_cast<int32>(EGameplayAbilityBindings::Cancel)));

	AbilityBindingsInitialized = true;
}