// ZombieAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class HM08_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	void TracePlayer();
	UPROPERTY()
	class AZombie* ControlledZombie;
	FTimerHandle TraceTimerHandle;
	virtual void OnPossess(APawn* InPawn) override;
};
