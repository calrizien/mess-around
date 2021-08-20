// Copyright Brandon Winston 2021


#include "BaseCharacter.h"
#include "BaseGameplayAbility.h"
#include "BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// Create the attribute set, this replicates by default
	//AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
	
	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ABaseCharacter::GiveDefaultAbilities()
{
	if (AbilitySystemComponent->DefaultAbilitiesGiven || GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	for (TSubclassOf<UBaseGameplayAbility>& DefaultAbility : DefaultAbilities)
	{
		if (IsValid(DefaultAbility))
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility, 1, static_cast<int32>(DefaultAbility.GetDefaultObject()->InputBinding), this));
		}
	}

	AbilitySystemComponent->DefaultAbilitiesGiven = true;
	if(AbilitySystemComponent->DefaultEffectsApplied == true && AbilitySystemComponent->DefaultAbilitiesGiven == true)
	{
		bAbilitiesInitialized = true;
	}
}

void ABaseCharacter::ApplyDefaultEffects()
{
	if (AbilitySystemComponent->DefaultEffectsApplied || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (TSubclassOf<UGameplayEffect> Effect : DefaultEffects)
	{
		if (IsValid(Effect))
		{
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContextHandle);

			if (GameplayEffectSpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent.Get());
			}
		}
	}	

	AbilitySystemComponent->DefaultEffectsApplied = true;
	if(AbilitySystemComponent->DefaultEffectsApplied == true && AbilitySystemComponent->DefaultAbilitiesGiven == true)
	{
		bAbilitiesInitialized = true;
	}
}

void ABaseCharacter::On_Death_Implementation()
{
	// TODO: Add On Death logic here!
}

void ABaseCharacter::GiveGameplayAbility_Implementation(TSubclassOf<UBaseGameplayAbility> AbilityToGive)
{
	if (GetLocalRole() != ROLE_Authority || !AbilityToGive || !AbilitySystemComponent.Get())
	{
		return;
	}

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGive, 1, static_cast<int32>(AbilityToGive.GetDefaultObject()->InputBinding), this));
}

bool ABaseCharacter::GiveGameplayAbility_Validate(TSubclassOf<UBaseGameplayAbility> AbilityToGive)
{
	return true;
}

void ABaseCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent.Get())
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CharacterLevel);
}


float ABaseCharacter::GetHealth() const
{
	if (!AttributeSet.Get())
		return 1.f;

	return AttributeSet->GetHealth();
}

float ABaseCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ABaseCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float ABaseCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ABaseCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 ABaseCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool ABaseCharacter::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities

		//RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		//AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

// // Called when the game starts or when spawned
// void ABaseCharacter::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
	const FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ABaseCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	// We only call the BP callback if this is not the initial ability setup
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ABaseCharacter::HandleManaChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnManaChanged(DeltaValue, EventTags);
	}
}

void ABaseCharacter::HandleMoveSpeedChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (bAbilitiesInitialized)
	{
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

/*void ABaseCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent.IsValid());
	
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// Grant abilities, but only on the server	
		for (TSubclassOf<UBaseGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}

		// Now apply passives
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
			}
		}
		// ARPG Inventory Function
		// AddSlottedGameplayAbilities();

		bAbilitiesInitialized = true;
	}
}*/

void ABaseCharacter::RemoveStartupGameplayAbilities()
{
	check(AbilitySystemComponent.IsValid());

	if (GetLocalRole() == ROLE_Authority && bAbilitiesInitialized)
	{
		// Remove any abilities added from a previous call
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && GameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		// Do in two passes so the removal happens after we have the full list
		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		// Remove all of the passive gameplay effects that were applied by this character
		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystemComponent->RemoveActiveEffects(Query);
// ARPG Inventory Function
		// RemoveSlottedGameplayAbilities(true);

		bAbilitiesInitialized = false;
	}
}

