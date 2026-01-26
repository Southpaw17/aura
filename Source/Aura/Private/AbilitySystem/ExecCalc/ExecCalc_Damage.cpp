// 


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResistance)

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;

	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput&           OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor*                        SourceAvatar = SourceASC->GetAvatarActor();
	AActor*                        TargetAvatar = TargetASC->GetAvatarActor();
	const ICombatInterface*        SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface*        TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec&   EffectSpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

	// Get Damage Set by Caller Magnitude
	float IncomingDamage = EffectSpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().IncomingDamage);
	IncomingDamage = FMath::Max(0.f, IncomingDamage);

	// --- Offensive Modifiers -------------------------------------------------
	// ----- Critical Strike ---------------------------------------------------
	float CritChanceMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvalParams,
	                                                           CritChanceMagnitude);
	CritChanceMagnitude = FMath::Max(0.f, CritChanceMagnitude);

	float CritResistanceMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResistanceDef, EvalParams,
	                                                           CritResistanceMagnitude);
	CritResistanceMagnitude = FMath::Max(0.f, CritResistanceMagnitude);

	const FRealCurve* CritResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("CritResistance"), FString());
	const float CritResistanceCoeff = CritResistanceCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	CritChanceMagnitude *= CritChanceMagnitude / (CritChanceMagnitude + CritResistanceMagnitude * CritResistanceCoeff);

	const float CritRoll = FMath::RandRange(0.f, 100.f);

	if (const bool bCritHit = CritRoll <= CritChanceMagnitude)
	{
		float CritDamageMagnitude = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvalParams,
		                                                           CritDamageMagnitude);
		CritDamageMagnitude = FMath::Max(0.f, CritDamageMagnitude);

		const float BonusDamageFromCrit = IncomingDamage * (1.f + (CritDamageMagnitude / 100.f));
		IncomingDamage += BonusDamageFromCrit;
	}

	// --- Defensive Modifiers -------------------------------------------------
	// ----- Block -------------------------------------------------------------

	// Capture Block Chance on Target and determine if there was a successful block
	// If Block, halve the damage.
	float BlockChanceMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvalParams,
	                                                           BlockChanceMagnitude);
	BlockChanceMagnitude = FMath::Max(0.f, BlockChanceMagnitude);

	const float BlockRoll = FMath::RandRange(0.f, 100.f);


	if (const bool bBlocked = BlockRoll <= BlockChanceMagnitude)
	{
		IncomingDamage *= 0.5f;
	}

	// ----- Armor & Armor Penetration -----------------------------------------
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoeff = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetCharacterLevel());

	const FRealCurve* ArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("EffectiveArmor"), FString());
	const float ArmorCoeff = ArmorCurve->Eval(TargetCombatInterface->GetCharacterLevel());

	// Armor Penetration ignores a percentage of the Target's Armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalParams, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvalParams,
	                                                           SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100.f;

	IncomingDamage *= (100 - EffectiveArmor * ArmorCoeff) / 100.f;

	// --- Final Application ---------------------------------------------------

	// Ensure hits always deal at least 1 damage.
	IncomingDamage = FMath::Max(1.f, IncomingDamage);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::AddBase, IncomingDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
