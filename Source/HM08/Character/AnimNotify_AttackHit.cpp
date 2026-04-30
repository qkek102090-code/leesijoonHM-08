#include "AnimNotify_AttackHit.h"
#include "Zombie.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AZombie* Zombie = Cast<AZombie>(MeshComp->GetOwner());
	if (Zombie)
	{
		Zombie->AttackHit();
	}
}