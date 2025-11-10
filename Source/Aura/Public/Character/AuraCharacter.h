#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	// --- Combat Interface ----------------------------------------------------
	virtual int32 GetCharacterLevel() const override;

	// --- Ability System Interface --------------------------------------------
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void InitAbilityActorInfo() override;
};
