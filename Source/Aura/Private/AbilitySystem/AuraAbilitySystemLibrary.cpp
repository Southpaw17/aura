#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
