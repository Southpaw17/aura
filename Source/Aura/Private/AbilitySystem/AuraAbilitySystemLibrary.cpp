#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
	AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet* AttributeSet = PS->GetAttributeSet();
	return FWidgetControllerParams(PlayerController, PS, ASC, AttributeSet);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC, float Level)
{
	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	
	if (!GameMode)
	{
		return;
	}

	UCharacterClassInfo* CharacterClassInfo = GameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	ApplyGameplayEffectToActor(ASC, ClassDefaultInfo.PrimaryAttributes, Level);
	ApplyGameplayEffectToActor(ASC, CharacterClassInfo->SecondaryAttributes, Level);
	ApplyGameplayEffectToActor(ASC, CharacterClassInfo->VitalAttributes, Level);
}

void UAuraAbilitySystemLibrary::ApplyGameplayEffectToActor(UAbilitySystemComponent* ASC,
	TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(AvatarActor);
	
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, Context);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
