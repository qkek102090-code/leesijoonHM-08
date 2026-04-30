#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Zombie.generated.h"

UCLASS()
class HM08_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();

	virtual void BeginPlay() override;

	void Attack();
	void AttackHit();

	float GetAttackRange() const { return AttackRange; }
protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* AttackMontage;

	bool bIsAttacking = false;
private:
	bool bHasHit = false;

	FTimerHandle AttackTimerHandle;

	void ResetAttack();
};