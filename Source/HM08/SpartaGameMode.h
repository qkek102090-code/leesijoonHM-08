// SpartaGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpartaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HM08_API ASpartaGameMode : public AGameMode
{
	GENERATED_BODY()
public:
    ASpartaGameMode();

protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditDefaultsOnly, Category = "Zombie")

    TSubclassOf<class AZombie> ZombieClass;

    void SpawnZombie();
    void SpawnZombiesForStage(int32 Stage);
    void ClearAllZombies();
    FTimerHandle ZombieSpawnTimerHandle;
    void StartRandomZombieSpawn();
    void SpawnZombieRandomly();
private:
    void HandleMainTimerElapsed();
    int32 MaxPlayTime;
    int32 CurrentStage;
    FTimerHandle MainTimerHandle;
    int32 AccumulatedSeconds;
    int32 AccumulatedMinute;
    float Score;
    UPROPERTY()
    TObjectPtr<class ASpartaHUD> CachedSpartaHUD;
};
