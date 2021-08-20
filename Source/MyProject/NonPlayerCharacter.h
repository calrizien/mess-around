// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "NonPlayerCharacter.generated.h"

/**
* Non Player Character is the base class to use for any Server controlled Characters.
* This class contains it's own Ability System Component and Attribute Sets.
 */
UCLASS(Blueprintable)
class MYPROJECT_API ANonPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	public:

	// Sets default values for this character's properties
	ANonPlayerCharacter();

	UPROPERTY(BlueprintReadOnly, Category = "Ability System")
	class UBaseAbilitySystemComponent* NPC_AbilitySystemComponent;

	// TODO: Create an NPC-specific attribute set and assign it here
	UPROPERTY(BlueprintReadOnly, Category = "Ability System")
	class UBaseAttributeSet* NPC_Attributes;


	protected:

	virtual void BeginPlay() override;
};
