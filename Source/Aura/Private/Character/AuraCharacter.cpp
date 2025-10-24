#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const
{
	return GetPlayerState<AAuraPlayerState>()->GetAbilitySystemComponent();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	if (AAuraPlayerState* PS = GetPlayerState<AAuraPlayerState>())
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		AttributeSet = PS->GetAttributeSet();
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Set up ASC for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Set up ASC for the Server
	InitAbilityActorInfo();
}
