// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Attribute Set intended to be used by all creatures with stats
 */
UCLASS()
class MYPROJECT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	// Constructor and overrides
	UBaseAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// TODO: Reimplement this function when ready. Currently throws a bunch of errors but also probably don't have the required imports yet
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	/** MaxHealth is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

	/** Health Regeneration Rate can be enhanced by effects */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegen)

	/** Current Mana, used to execute special abilities. Capped by MaxMana */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing=OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana)

	/** MaxMana is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana)
	
	/** Mana Regeneration Rate can be enhanced by effects */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing=OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ManaRegen)


	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackPower)

	/** Base Damage is divided by DefensePower to get actual damage done, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DefensePower)

	/** MoveSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)

	// Strength
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength)
	
	// Stamina
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)
	
	// Intelligence
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Intelligence)
	
	// Wisdom
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Wisdom)
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Wisdom)
	
	// Charisma
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Charisma)
	FGameplayAttributeData Charisma;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Charisma)
	
	// Agility
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Agility)
	
	// Dexterity
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Dexterity)
	
	// Resist Magic
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_ResistMagic)
	FGameplayAttributeData ResistMagic;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ResistMagic)
	
	// Resist Poison
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_ResistPoison)
	FGameplayAttributeData ResistPoison;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ResistPoison)
	
	// Resist Disease
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_ResistDisease)
	FGameplayAttributeData ResistDisease;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ResistDisease)
	
	// Resist Cold
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_ResistCold)
	FGameplayAttributeData ResistCold;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ResistCold)
	
	// Resist Fire
	UPROPERTY(BlueprintReadOnly, Category = "Characteristics", ReplicatedUsing = OnRep_ResistFire)
	FGameplayAttributeData ResistFire;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ResistFire)

	// Experience Points used to determine character level
	UPROPERTY(BlueprintReadOnly, Category = "Experience", ReplicatedUsing = OnRep_ExperiencePoints)
	FGameplayAttributeData ExperiencePoints;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ExperiencePoints)

	// Character Level - will be determined by Experience Points for PCs and set manually for NPCs
	UPROPERTY(BlueprintReadOnly, Category = "Experience", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CharacterLevel)

	protected:
	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	// These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ManaRegen(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DefensePower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Wisdom(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Charisma(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Agility(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ResistMagic(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ResistPoison(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ResistDisease(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ResistCold(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_ResistFire(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ExperiencePoints(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldValue);

	// Hardcoded Max Level. TODO: Refactor for this to be configable
	float MaxLevel = 50.0;
	
	// We don't want high-level mobs to be exploitable by lower level characters so we cap the amount of XP a mob can possibly give to a character. TODO: Refactor when XP curves are implemented
	float GetMaxXPPerMob(float CharacterLevel);
	
};
