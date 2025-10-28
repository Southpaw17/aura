

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() = default;
	explicit FEffectProperties(const FGameplayEffectModCallbackData& Data);

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	// --- Attributes ----------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthMax, Category = "Attributes")
	FGameplayAttributeData HealthMax;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, HealthMax);

	UFUNCTION()
	void OnRep_HealthMax(const FGameplayAttributeData& OldValue) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaMax, Category = "Attributes")
	FGameplayAttributeData ManaMax;
	ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, ManaMax);

	UFUNCTION()
	void OnRep_ManaMax(const FGameplayAttributeData& OldValue) const;
	
	
};
