// Copyright Brandon Winston 2021


#include "BaseGameplayAbility.h"
#include "BaseAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	// Sets the ability to default to Instanced Per Actor.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;	
}

void UBaseGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UBaseGameplayAbility::ApplyGameplayEffectToOwner(TSubclassOf<UGameplayEffect> EffectToAdd, float EffectLevel)
{
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectToAdd, EffectLevel, EffectContextHandle);

	if (GameplayEffectSpecHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), GetAbilitySystemComponentFromActorInfo());
	}
}

void UBaseGameplayAbility::ApplyGameplayEffectToTarget(TSubclassOf<UGameplayEffect> EffectToAdd, AActor* Target,
	float EffectLevel)
{
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectToAdd, EffectLevel, EffectContextHandle);

	
	if (GameplayEffectSpecHandle.IsValid())
	{
		const bool ImplementsInterface = UKismetSystemLibrary::DoesImplementInterface(Target, UAbilitySystemInterface::StaticClass());

		if (ImplementsInterface)
		{
			IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(Target);
		
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), Interface->GetAbilitySystemComponent());
		}
	}
}

void UBaseGameplayAbility::ApplySetByCallerGameplayEffect(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffect,
	float EffectLevel, FGameplayTag SetByCallerTag, float SetByCallerAmount)
{
	UAbilitySystemComponent* OwningAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle EffectContextHandle = OwningAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = OwningAbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, EffectLevel, EffectContextHandle);
	
	if (GameplayEffectSpecHandle.IsValid())
	{
		const bool ImplementsInterface = UKismetSystemLibrary::DoesImplementInterface(Target, UAbilitySystemInterface::StaticClass());

		if (ImplementsInterface)
		{
			IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(Target);
			
			GameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(SetByCallerTag, SetByCallerAmount);

			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), Interface->GetAbilitySystemComponent());
		}
	}	
}

float UBaseGameplayAbility::GetModifiedAbilityStrength()
{
	float BaseStrength = BaseAbilityStrength.GetValueAtLevel(GetAbilityLevel());

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	
	for (FAbilityModifierStruct Modifier : AbilityModifiers)
	{
		const float AttributeValue = AbilitySystemComponent->GetNumericAttribute(Modifier.BackingAttribute);

		BaseStrength += AttributeValue * Modifier.AttributeModifier;
	}

	return BaseStrength;	
}



