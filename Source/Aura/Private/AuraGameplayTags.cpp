// 


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// --- Primary Attribute Tags ----------------------------------------------
	Instance.Attributes_Primary_Strength = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
	                                                                    FString(""));
	Instance.Attributes_Primary_Intelligence = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), FString(""));
	Instance.Attributes_Primary_Resilience = Manager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), FString(""));
	Instance.Attributes_Primary_Vigor = Manager.AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString(""));

	// --- Secondary Attribute Tags --------------------------------------------
	Instance.Attributes_Secondary_Armor = Manager.
		AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString(""));
	Instance.Attributes_Secondary_ArmorPenetration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), FString(""));
	Instance.Attributes_Secondary_BlockChance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), FString(""));
	Instance.Attributes_Secondary_CritChance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CritChance"), FString(""));
	Instance.Attributes_Secondary_CritDamage = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CritDamage"), FString(""));
	Instance.Attributes_Secondary_CritResistance = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CritResistance"), FString(""));
	Instance.Attributes_Secondary_HealthRegeneration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), FString(""));
	Instance.Attributes_Secondary_ManaRegeneration = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), FString(""));
	Instance.Attributes_Secondary_HealthMax = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthMax"), FString(""));
	Instance.Attributes_Secondary_ManaMax = Manager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaMax"), FString(""));

	// --- Damage Resistance ---------------------------------------------------
	Instance.Attributes_Resistance_Fire = Manager.
		AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString(""));
	Instance.Attributes_Resistance_Lightning = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"), FString(""));
	Instance.Attributes_Resistance_Arcane = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"), FString(""));
	Instance.Attributes_Resistance_Physical = Manager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"), FString(""));

	// --- Vital Attribute Tags ------------------------------------------------
	Instance.Attributes_Vital_Health = Manager.AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString(""));
	Instance.Attributes_Vital_Mana = Manager.AddNativeGameplayTag(FName("Attributes.Vital.Mana"), FString(""));

	// --- Input Tags ----------------------------------------------------------
	Instance.InputTag_LMB = Manager.AddNativeGameplayTag(FName("InputTag.LMB"), FString(""));
	Instance.InputTag_RMB = Manager.AddNativeGameplayTag(FName("InputTag.RMB"), FString(""));
	Instance.InputTag_1 = Manager.AddNativeGameplayTag(FName("InputTag.1"), FString(""));
	Instance.InputTag_2 = Manager.AddNativeGameplayTag(FName("InputTag.2"), FString(""));
	Instance.InputTag_3 = Manager.AddNativeGameplayTag(FName("InputTag.3"), FString(""));
	Instance.InputTag_4 = Manager.AddNativeGameplayTag(FName("InputTag.4"), FString(""));

	// --- Gameplay Effect Tags ------------------------------------------------
	Instance.IncomingDamage = Manager.AddNativeGameplayTag(FName("GameplayEffects.IncomingDamage"), FString(""));
	Instance.Effects_HitReact = Manager.AddNativeGameplayTag(FName("Effects.HitReact"), FString(""));

	// --- Damage Types --------------------------------------------------------
	Instance.Damage_Fire = Manager.AddNativeGameplayTag(FName("Damage.Fire"), FString(""));
	Instance.Damage_Lightning = Manager.AddNativeGameplayTag("Damage.Lightning", FString(""));
	Instance.Damage_Arcane = Manager.AddNativeGameplayTag("Damage.Arcane", FString(""));
	Instance.Damage_Physical = Manager.AddNativeGameplayTag("Damage.Physical", FString(""));
	
	// Must add Damage Types to this map to be able to loop over them in ExecCalc_Damage
	Instance.DamageTypesToResistance.Add(Instance.Damage_Fire, Instance.Attributes_Resistance_Fire);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Lightning, Instance.Attributes_Resistance_Lightning);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Arcane, Instance.Attributes_Resistance_Arcane);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Physical, Instance.Attributes_Resistance_Physical);
}
