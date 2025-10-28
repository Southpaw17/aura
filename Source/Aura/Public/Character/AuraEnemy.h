#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/InteractableInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	// --- Interactable Interface ----------------------------------------------
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	// -------------------------------------------------------------------------
	virtual void BeginPlay() override;

protected:
	virtual void InitAbilityActorInfo() override;
};
