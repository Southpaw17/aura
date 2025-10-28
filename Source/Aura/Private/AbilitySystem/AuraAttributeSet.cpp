#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

FEffectProperties::FEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	EffectContextHandle = Data.EffectSpec.GetContext();

	SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(SourceASC))
	{
		SourceAvatarActor = SourceASC->GetAvatarActor();

		if (SourceAvatarActor)
		{
			if (ACharacter* Character = Cast<ACharacter>(SourceAvatarActor))
			{
				SourceCharacter = Character;
				SourceController = Character->GetController();
			}
		}
	}

	TargetASC = &Data.Target;

	if (IsValid(TargetASC))
	{
		TargetAvatarActor = TargetASC->GetAvatarActor();

		if (TargetAvatarActor)
		{
			if (ACharacter* Character = Cast<ACharacter>(TargetAvatarActor))
			{
				TargetCharacter = Character;
				TargetController = Character->GetController();
			}
		}
	}
}

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitHealthMax(100.f);
	InitMana(50.f);
	InitManaMax(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetHealthMax());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetManaMax());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties(Data);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldValue);
}

void UAuraAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthMax, OldValue);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldValue);
}

void UAuraAttributeSet::OnRep_ManaMax(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaMax, OldValue);
}
