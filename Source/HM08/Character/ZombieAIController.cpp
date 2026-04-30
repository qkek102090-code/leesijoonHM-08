// ZombieAIController.cpp


#include "HM08/Character/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "HM08/Character/Zombie.h"
#include "HMCharacter.h"

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TraceTimerHandle,
		this,
		&AZombieAIController::TracePlayer,
		0.5f,
		true
	);
}


void AZombieAIController::TracePlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());

	AZombie* Zombie = Cast<AZombie>(GetPawn());
	if (!Zombie) return;

	if (Distance <= Zombie->GetAttackRange())
	{
		Zombie->Attack();
	}
	else
	{
		MoveToActor(PlayerPawn, 50.f);
	}
}
void AZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledZombie = Cast<AZombie>(InPawn);

	if (ControlledZombie)
	{
		UE_LOG(LogTemp, Warning, TEXT("좀비 Possess 성공"));
	}
}

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AZombie* Zombie = Cast<AZombie>(GetPawn());
	if (!Zombie) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(
		Zombie->GetActorLocation(),
		PlayerPawn->GetActorLocation()
	);

	// 공격 범위 안이면 공격
	if (Distance <= Zombie->GetAttackRange())
	{
		Zombie->Attack();
	}
}