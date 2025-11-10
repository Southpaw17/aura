

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_ManaMax.generated.h"


UCLASS()
class AURA_API UMMC_ManaMax : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_ManaMax();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
