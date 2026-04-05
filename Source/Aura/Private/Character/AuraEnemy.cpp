#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(GetRootComponent());
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// AI only runs on the server, so no need to continue below here
	if (!HasAuthority()) return;
	
	AuraAIController = Cast<AAuraAIController>(NewController);
	
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("RangedAttacker", CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

int32 AAuraEnemy::GetCharacterLevel() const
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	if (HasAuthority())
	{
		InitAbilityActorInfo();
	}

	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

	UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthWidget->GetUserWidgetObject());
	if (AuraUserWidget)
	{
		AuraUserWidget->SetWidgetController(this);
	}

	const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthMaxAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthMaxChanged.Broadcast(Data.NewValue);
		});

	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Effects_HitReact).AddUObject(
		this, &AAuraEnemy::HitReactTagChanged);

	OnHealthChanged.Broadcast(AuraAS->GetHealth());
	OnHealthMaxChanged.Broadcast(AuraAS->GetHealthMax());
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("HitReacting", bHitReacting);
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, AbilitySystemComponent, Level);
	}
}
