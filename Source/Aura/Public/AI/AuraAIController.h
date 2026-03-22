// 

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class AURA_API AAuraAIController : public AAIController
{
protected:

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraAIController();

protected:		
	UPROPERTY()
	UBehaviorTreeComponent* BehaviorTreeComponent;
};
