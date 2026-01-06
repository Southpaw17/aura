


#include "Input/AuraInputConfig.h"

DECLARE_LOG_CATEGORY_CLASS(AuraInputConfig, Log, Log)

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Config : AbilityInputActions)
	{
		if (Config.InputAction && Config.InputTag.MatchesTagExact(Tag))
		{
			return Config.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(AuraInputConfig, Error, TEXT("Unable to find Input Action for Tag [%s], on InputConfig [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
