// 

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return Instance; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;

protected:

private:
	FAuraGameplayTags() = default;
	FAuraGameplayTags(const FAuraGameplayTags&) = delete;
	FAuraGameplayTags(FAuraGameplayTags&&) = delete;
	FAuraGameplayTags& operator=(const FAuraGameplayTags&) = delete;
	FAuraGameplayTags& operator=(FAuraGameplayTags&&) = delete;
	
	static FAuraGameplayTags Instance;
};
