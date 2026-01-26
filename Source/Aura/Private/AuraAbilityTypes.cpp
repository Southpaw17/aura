#include "AuraAbilityTypes.h"

enum RepMask : uint32
{
	Instigator      = 1 << 0,
	EffectCauser    = 1 << 1,
	AbilityCDO      = 1 << 2,
	SourceObject    = 1 << 3,
	Actors          = 1 << 4,
	HitResult       = 1 << 5,
	bHasWorldOrigin = 1 << 6,
	bIsBlockingHit  = 1 << 7,
	bIsCriticalHit  = 1 << 8,
};

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;

	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= RepMask::Instigator;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= RepMask::EffectCauser;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= RepMask::AbilityCDO;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= RepMask::SourceObject;
		}
		if (Actors.Num())
		{
			RepBits |= RepMask::Actors;
		}
		if (HitResult.IsValid())
		{
			RepBits |= RepMask::HitResult;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= RepMask::bHasWorldOrigin;
		}
		if (bIsBlockingHit)
		{
			RepBits |= RepMask::bIsBlockingHit;
		}
		if (bIsCriticalHit)
		{
			RepBits |= RepMask::bIsCriticalHit;
		}
	}
	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & RepMask::Instigator)
	{
		Ar << Instigator;
	}

	if (RepBits & RepMask::EffectCauser)
	{
		Ar << EffectCauser;
	}

	if (RepBits & RepMask::AbilityCDO)
	{
		Ar << AbilityCDO;
	}

	if (RepBits & RepMask::SourceObject)
	{
		Ar << SourceObject;
	}

	if (RepBits & RepMask::Actors)
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & RepMask::HitResult)
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = MakeShared<FHitResult>();
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & RepMask::bHasWorldOrigin)
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & RepMask::bIsBlockingHit)
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & RepMask::bIsCriticalHit)
	{
		Ar << bIsCriticalHit;
	}
	
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get());  // Just to initialize InstigatorAbilitySystemComponent
	}
	
	bOutSuccess = true;
	return true;
}
