// Copyright Brandon Winston 2021

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "BaseAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "BaseAttributeSet.h"
#include "BaseCharacter.generated.h"

class UBaseGamePlayAbility;
class UGameplayEffect;

// Character Base is a template class that contains all shared logic for "Player Characters" and "Non Player Characters".
// This Class is Abstract and should not be used directly! (Non-Blueprintable)

UCLASS(Abstract, NotBlueprintable)
class MYPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void OnRep_Controller() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Implement IAbilitySystemInterface. (This is used to find the Ability System Component)
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// The default Abilities this Character has access to.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefaultAbilities;

	// The default gameplay Effects this Character has when spawned.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	// Add logic here that applies when the Character reaches 0 health.
	UFUNCTION(BlueprintNativeEvent, Category = "Character Base")
	void On_Death();

	// Gives the supplied Ability to this Character.
	// Should be called on the Server!
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Server, Reliable, WithValidation, Category = "Character Base|Ability System")
	void GiveGameplayAbility(TSubclassOf<UBaseGameplayAbility> AbilityToGive);

	// ... Old ARPG Functions
	
	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** Modifies the character level, this may change abilities. Returns true on success */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

protected:
	// ...
	// Creates a pointer to the Ability System Component associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	TWeakObjectPtr<class UBaseAbilitySystemComponent> AbilitySystemComponent;

	// Creates a pointer to the Attribute Set associated with this Character.
	// Player Characters will set this in OnRep_PlayerState() locally, and in OnPossessed() server side.
	// Non Player Characters will set this in its constructor.
	TWeakObjectPtr<class UBaseAttributeSet> AttributeSet;

	// Called to give the Character it's Default Abilities from the "Default Abilities" array.
	void GiveDefaultAbilities();

	// Called to apply the Gameplay Effects in the "Default Effects" array.
	void ApplyDefaultEffects();



	/**
	* Called when character takes damage, which may have killed them
	*
	* @param DamageAmount Amount of damage that was done, not clamped based on current health
	* @param HitInfo The hit info that generated this damage
	* @param DamageTags The gameplay tags of the event that did the damage
	* @param InstigatorCharacter The character that initiated this damage
	* @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);

	/**
	* Called when health is changed, either from healing or from being damaged
	* For damage this is called in addition to OnDamaged/OnKilled
	*
	* @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	* @param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	* Called when mana is changed, either from healing or from being used as a cost
	*
	* @param DeltaValue Change in mana value, positive for heal, negative for cost. If 0 the delta is unknown
	* @param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	* Called when movement speed is changed
	*
	* @param DeltaValue Change in move speed
	* @param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** Apply the startup gameplay abilities and effects -Now split into two functions for abilities and passives.*/
	// void AddStartupGameplayAbilities();

	/** Attempts to remove any startup gameplay abilities */
	void RemoveStartupGameplayAbilities();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	// Friended to allow access to handle functions above
	friend UBaseAttributeSet;

	
	protected:
	//  Old ARPG Functions Might Delete...
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;


	// The level of this character, should not be modified directly once it has already spawned
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UBaseGameplayAbility>> GameplayAbilities;

	// Passive gameplay effects applied on creation 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;


	// replaced the following two properties with weak pointer types above
	// // The component used to handle ability system interactions
	// UPROPERTY()
	// UBaseAbilitySystemComponent* AbilitySystemComponent;
	
	// // List of attributes modified by the ability system 
	// UPROPERTY()
	// UBaseAttributeSet* AttributeSet; 

	// If true we have initialized our abilities 
	UPROPERTY()
	int32 bAbilitiesInitialized;
};
