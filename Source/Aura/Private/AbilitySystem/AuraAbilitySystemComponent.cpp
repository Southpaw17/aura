


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
                                                  const FActiveGameplayEffectHandle Handle)
{
	FGameplayTagContainer AssetTagContainer;
	Spec.GetAllAssetTags(AssetTagContainer);

	EffectAssetTags.Broadcast(AssetTagContainer);
}
