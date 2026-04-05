#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

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
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritChance, GetCritChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritDamage, GetCritDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritResistance, GetCritResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthMax, GetHealthMaxAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaMax, GetManaMaxAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetResistanceArcaneAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ResistancePhysical, COND_None, REPNOTIFY_Always);
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
	
	constexpr float MaxResistance = 75.f;
	if (Attribute == GetResistanceFireAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxResistance);
	}
	
	if (Attribute == GetResistanceLightningAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxResistance);
	}
	
	if (Attribute == GetResistanceArcaneAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxResistance);
	}
	
	if (Attribute == GetResistancePhysicalAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, MaxResistance);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetHealthMax()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetManaMax()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage > 0)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetHealthMax()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bIsBlockedHit = UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			const bool bIsCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
			ShowFloatingText(EffectProperties, LocalIncomingDamage, bIsBlockedHit, bIsCriticalHit);
		}
	}
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldValue);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldValue);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldValue);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldValue);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldValue);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldValue);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldValue);
}

void UAuraAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritChance, OldValue);
}

void UAuraAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritDamage, OldValue);
}

void UAuraAttributeSet::OnRep_CritResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritResistance, OldValue);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldValue);
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

void UAuraAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceFire, OldValue);
}

void UAuraAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceLightning, OldValue);
}

void UAuraAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistanceArcane, OldValue);
}

void UAuraAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ResistancePhysical, OldValue);
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties,
                                         float                    Damage,
                                         bool                     bIsBlockedHit,
                                         bool                     bIsCriticalHit) const
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(
			EffectProperties.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(EffectProperties.TargetCharacter, Damage, bIsBlockedHit, bIsCriticalHit);
			return;
		}
		
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(
			EffectProperties.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(EffectProperties.TargetCharacter, Damage, bIsBlockedHit, bIsCriticalHit);
		}
	}
}
