// 


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// --- Primary Attribute Tags ----------------------------------------------
	Instance.Attributes_Primary_Strength = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Strength"));
	Instance.Attributes_Primary_Intelligence = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"));
	Instance.Attributes_Primary_Resilience = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Resilience"));
	Instance.Attributes_Primary_Vigor = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Vigor"));

	// --- Secondary Attribute Tags --------------------------------------------
	Instance.Attributes_Secondary_Armor = Manager.
		AddNativeGameplayTag(FName("Attributes.Secondary.Armor"));
	Instance.Attributes_Secondary_ArmorPenetration = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"));
	Instance.Attributes_Secondary_BlockChance = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"));
	Instance.Attributes_Secondary_CritChance = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.CritChance"));
	Instance.Attributes_Secondary_CritDamage = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.CritDamage"));
	Instance.Attributes_Secondary_CritResistance = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.CritResistance"));
	Instance.Attributes_Secondary_HealthRegeneration = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"));
	Instance.Attributes_Secondary_ManaRegeneration = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"));
	Instance.Attributes_Secondary_HealthMax = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.HealthMax"));
	Instance.Attributes_Secondary_ManaMax = Manager.AddNativeGameplayTag(FName("Attributes.Secondary.ManaMax"));

	// --- Damage Resistance ---------------------------------------------------
	Instance.Attributes_Resistance_Fire = Manager.AddNativeGameplayTag(FName("Attributes.Resistance.Fire"));
	Instance.Attributes_Resistance_Lightning = Manager.AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"));
	Instance.Attributes_Resistance_Arcane = Manager.AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"));
	Instance.Attributes_Resistance_Physical = Manager.AddNativeGameplayTag(FName("Attributes.Resistance.Physical"));

	// --- Vital Attribute Tags ------------------------------------------------
	Instance.Attributes_Vital_Health = Manager.AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	Instance.Attributes_Vital_Mana = Manager.AddNativeGameplayTag(FName("Attributes.Vital.Mana"));

	// --- Input Tags ----------------------------------------------------------
	Instance.InputTag_LMB = Manager.AddNativeGameplayTag(FName("InputTag.LMB"));
	Instance.InputTag_RMB = Manager.AddNativeGameplayTag(FName("InputTag.RMB"));
	Instance.InputTag_1 = Manager.AddNativeGameplayTag(FName("InputTag.1"));
	Instance.InputTag_2 = Manager.AddNativeGameplayTag(FName("InputTag.2"));
	Instance.InputTag_3 = Manager.AddNativeGameplayTag(FName("InputTag.3"));
	Instance.InputTag_4 = Manager.AddNativeGameplayTag(FName("InputTag.4"));

	// --- Gameplay Effect Tags ------------------------------------------------
	Instance.IncomingDamage = Manager.AddNativeGameplayTag(FName("GameplayEffects.IncomingDamage"));
	Instance.Effects_HitReact = Manager.AddNativeGameplayTag(FName("Effects.HitReact"));

	// --- Damage Types --------------------------------------------------------
	Instance.Damage_Fire = Manager.AddNativeGameplayTag(FName("Damage.Fire"));
	Instance.Damage_Lightning = Manager.AddNativeGameplayTag("Damage.Lightning");
	Instance.Damage_Arcane = Manager.AddNativeGameplayTag("Damage.Arcane");
	Instance.Damage_Physical = Manager.AddNativeGameplayTag("Damage.Physical");

	// --- Ability Tags --------------------------------------------------------
	Instance.Abilities_Attack = Manager.AddNativeGameplayTag(FName("Abilities.Attack"));

	// --- Montage Tags --------------------------------------------------------
	Instance.Montage_Attack_Weapon = Manager.AddNativeGameplayTag(FName("Montage.Attack.Weapon"));
	Instance.Montage_Attack_RightHand = Manager.AddNativeGameplayTag(FName("Montage.Attack.RightHand"));
	Instance.Montage_Attack_LeftHand = Manager.AddNativeGameplayTag(FName("Montage.Attack.LeftHand"));

	// Must add Damage Types to this map to be able to loop over them in ExecCalc_Damage
	Instance.DamageTypesToResistance.Add(Instance.Damage_Fire, Instance.Attributes_Resistance_Fire);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Lightning, Instance.Attributes_Resistance_Lightning);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Arcane, Instance.Attributes_Resistance_Arcane);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Physical, Instance.Attributes_Resistance_Physical);
}
