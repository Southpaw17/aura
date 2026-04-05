#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			const FWidgetControllerParams Params = CreateWidgetControllerParams(PC);
			return AuraHUD->GetOverlayWidgetController(Params);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			const FWidgetControllerParams Params = CreateWidgetControllerParams(PC);
			return AuraHUD->GetAttributeMenuWidgetController(Params);
		}
	}

	return nullptr;
}

FWidgetControllerParams UAuraAbilitySystemLibrary::CreateWidgetControllerParams(APlayerController* PlayerController)
{
	AAuraPlayerState*        PS = PlayerController->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet*           AttributeSet = PS->GetAttributeSet();
	return FWidgetControllerParams(PlayerController, PS, ASC, AttributeSet);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject*           WorldContextObject,
                                                            ECharacterClass          CharacterClass,
                                                            UAbilitySystemComponent* ASC,
                                                            float                    Level)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

	if (!CharacterClassInfo)
	{
		return;
	}

	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	ApplyGameplayEffectToActor(ASC, ClassDefaultInfo.PrimaryAttributes, Level);
	ApplyGameplayEffectToActor(ASC, CharacterClassInfo->SecondaryAttributes, Level);
	ApplyGameplayEffectToActor(ASC, CharacterClassInfo->VitalAttributes, Level);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject*           WorldContextObject,
                                                     UAbilitySystemComponent* ASC,
                                                     ECharacterClass          CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

	if (!CharacterClassInfo)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityToGrant : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityToGrant, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetCharacterLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (!GameMode)
	{
		return nullptr;
	}

	return GameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AuraContext->IsBlockedHit();
	}

	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}

	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                 bool                          bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject*        WorldContextObject,
                                                           TArray<AActor*>&      OutOverlappingActors,
                                                           const TArray<AActor*> ActorsToIgnore,
                                                           float                 Radius,
                                                           const FVector&        SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams::AllDynamicObjects,
		                                FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* OverlappedActor = Overlap.GetActor();
			if (OverlappedActor->Implements<UCombatInterface>())
			{
				if (!ICombatInterface::Execute_IsDead(OverlappedActor))
				{
					OutOverlappingActors.AddUnique(OverlappedActor);
				}
			}
		}
	}
}

void UAuraAbilitySystemLibrary::ApplyGameplayEffectToActor(UAbilitySystemComponent*     ASC,
                                                           TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                                           float                        Level)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(AvatarActor);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, Context);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
