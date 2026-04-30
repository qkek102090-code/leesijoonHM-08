#include "HM08/Character/Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "HMCharacter.h"
#include "Animation/AnimInstance.h"
#include "HM08/Character/ZombieAIController.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AZombieAIController::StaticClass();
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();
}

void AZombie::Attack()
{
	if (bIsAttacking) return;
	if (!AttackMontage) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	bIsAttacking = true;
	AnimInstance->Montage_Play(AttackMontage);

	UE_LOG(LogTemp, Warning, TEXT("좀비 공격 시작"));
}

void AZombie::ResetAttack()
{
	bIsAttacking = false;
}

void AZombie::AttackHit()
{
	UE_LOG(LogTemp, Warning, TEXT("공격 판정 실행됨"));

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

	if (Distance <= AttackRange)
	{
		AHMCharacter* Player = Cast<AHMCharacter>(PlayerPawn);
		if (Player)
		{
			Player->UpdateHP(-10.f);
			UE_LOG(LogTemp, Warning, TEXT("플레이어 피격!"));
		}
	}

	bIsAttacking = false;
}