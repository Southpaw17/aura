#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class IInteractableInterface;
class UInputAction;
class UInputMappingContext;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class ACharacter;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(ACharacter* TargetCharacter, float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftPressed = true; };
	void ShiftReleased() { bShiftPressed = false; };
	bool bShiftPressed = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	TScriptInterface<IInteractableInterface> LastActor;
	TScriptInterface<IInteractableInterface> ThisActor;
	FHitResult                               CursorHit;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UFUNCTION()
	void AbilityInputTagPressed(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float   FollowTime = 0.f;
	float   ShortPressThresholdSecs = 0.5f;
	bool    bAutoRunning = false;
	bool    bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;

	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
