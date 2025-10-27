#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	check(InGameplayEffectClass);
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
		Context.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpec = TargetASC->MakeOutgoingSpec(InGameplayEffectClass, 1.f, Context);

		FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
			*EffectSpec.Data.Get());

		const bool bIsInfinite = EffectSpec.Data.Get()->Def.Get()->DurationPolicy ==
			EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveGameplayEffects.Add(TargetASC, ActiveEffectHandle);
		}
	}
}

void AAuraEffectActor::OnOverlapBegin(AActor* TargetActor)
{
	switch (EffectApplicationPolicy)
	{
	case EEffectApplicationPolicy::ApplyOnOverlap:
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
			break;
		}
	case EEffectApplicationPolicy::ApplyOnEndOverlap:
	case EEffectApplicationPolicy::DoNotApply:
	default:
		{
		}
	}
}

void AAuraEffectActor::OnOverlapEnd(AActor* TargetActor)
{
	if (EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}

	if (EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		if (ActiveGameplayEffects.Contains(TargetASC))
		{
			FActiveGameplayEffectHandle* ActiveEffectHandle = ActiveGameplayEffects.Find(TargetASC);
			TargetASC->RemoveActiveGameplayEffect(*ActiveEffectHandle, 1);
			ActiveGameplayEffects.Remove(TargetASC);
		}
	}
}
