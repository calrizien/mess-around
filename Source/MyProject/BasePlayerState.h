// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "BaseAbilitySystemComponent.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ABasePlayerState();
	
	// The Ability System Component for this PlayerState. 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ability System")
	class UBaseAbilitySystemComponent* AbilitySystemComponent;

	// Implement the IAbilitySystemInterface. (This is used to find the Ability System Component)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// The Attribute Set on this Player State.
	UPROPERTY(BlueprintReadOnly, Category = "Ability System")
	class UBaseAttributeSet* Attributes;
};

// yeah