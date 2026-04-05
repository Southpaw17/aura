// 


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	for (TPair<FGameplayTag, FScalableFloat>& DamageTypePair : DamageTypes)
	{
		const FGameplayTag DamageType = DamageTypePair.Key;
		const float        DamageMagnitude = DamageTypePair.Value.GetValueAtLevel(GetAbilityLevel());

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, DamageMagnitude);
	}

	UAbilitySystemComponent* OwningASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	OwningASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}
