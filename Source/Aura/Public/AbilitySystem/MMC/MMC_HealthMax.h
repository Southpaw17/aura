

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_HealthMax.generated.h"


UCLASS()
class AURA_API UMMC_HealthMax : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_HealthMax();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
