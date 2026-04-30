// SpartaGameMode.cpp


#include "SpartaGameMode.h"
#include "SpartaHUD.h"
#include "HMCharacter.h"
#include "MyPlayerController.h"
#include "NavigationSystem.h"
#include "Character/Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
ASpartaGameMode::ASpartaGameMode()
{
    AccumulatedSeconds = 0;
    AccumulatedMinute = 0;
    MaxPlayTime = 60;
    CurrentStage = 1;
    Score = 0.0f;
    

    HUDClass = ASpartaHUD::StaticClass();

    DefaultPawnClass = AHMCharacter::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
}

void ASpartaGameMode::BeginPlay()
{
    Super::BeginPlay();

    SpawnZombiesForStage(CurrentStage);
    StartRandomZombieSpawn();
    GetWorld()->GetTimerManager().SetTimer(
        MainTimerHandle,
        this,
        &ThisClass::HandleMainTimerElapsed,
        1.f,
        true
    );

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        CachedSpartaHUD = Cast<ASpartaHUD>(PC->GetHUD());
    }
    UE_LOG(LogTemp, Error, TEXT("Stage-%02d started!"), CurrentStage);
}

void ASpartaGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);

    Super::EndPlay(EndPlayReason);
}

void ASpartaGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    Score += 0.1f;
    if (CachedSpartaHUD)
    {
        CachedSpartaHUD->UpdateScore(Score);
    }
}

void ASpartaGameMode::HandleMainTimerElapsed()
{
    AccumulatedSeconds += 1;
    int32 DisplayMin = AccumulatedSeconds / 60;
    int32 DisplaySec = AccumulatedSeconds % 60;
    int32 DisplayRound = CurrentStage;
    int32 DisplayMaxRound = 5;

    UE_LOG(LogTemp, Warning, TEXT("Main timer elapsed... [Stage-%d][%02ds][%02ds]"), CurrentStage, AccumulatedMinute, AccumulatedSeconds);
    int32 LivingZombies = 0;
    for (TActorIterator<AZombie> It(GetWorld()); It; ++It)
    {
        LivingZombies++;
    }
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC != nullptr)
    {
        ASpartaHUD* SpartaHUD = Cast<ASpartaHUD>(PC->GetHUD());
        if (SpartaHUD != nullptr)
        {
            // 매 초 피가다는 테스트용 로직
            AHMCharacter* MyChar = Cast<AHMCharacter>(PC->GetPawn());
            if (MyChar)
            {
                //MyChar->UpdateHP(-1.0f);
                SpartaHUD->UpdateHP(MyChar->GetCurrentHP(), MyChar->GetMaxHP());
            }
            SpartaHUD->UpdateTime(DisplayMin, DisplaySec);
            SpartaHUD->UpdateRound(DisplayRound, DisplayMaxRound);
            SpartaHUD->UpdateZombieCount(LivingZombies);
        }
    }

    if (AccumulatedSeconds % MaxPlayTime == 0)
    {
        CurrentStage += 1;
        AccumulatedSeconds = 0;
        AccumulatedMinute = 0;
        if (CurrentStage != 6)
        {
            ClearAllZombies();
            SpawnZombiesForStage(CurrentStage);
            UE_LOG(LogTemp, Warning, TEXT("Stage-%d started!"), CurrentStage)
        }
        else
        {
            if (PC != nullptr)
            {
                ASpartaHUD* SpartaHUD = Cast<ASpartaHUD>(PC->GetHUD());
                if (SpartaHUD != nullptr)
                {
                    SpartaHUD->ShowGameOverMenu();
                }
            }
        }
    }
}

void ASpartaGameMode::SpawnZombie()
{
    if (!ZombieClass) return;

    // 1. 내비게이션 시스템 가져오기
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
        FNavLocation RandomLocation;
        // 2. 0,0,0 좌표 기준 3000 유닛 반경 내 이동 가능한 랜덤 좌표 찾기
        if (NavSys->GetRandomReachablePointInRadius(FVector::ZeroVector, 3000.f, RandomLocation))
        {
            // 3. 좀비 스폰
            GetWorld()->SpawnActor<AZombie>(ZombieClass, RandomLocation.Location, FRotator::ZeroRotator);
        }
    }
}

void ASpartaGameMode::SpawnZombiesForStage(int32 Stage)
{
    if (!ZombieClass) return;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) return;

    // 스테이지당 스폰할 좀비 수 (예: 스테이지 1에 5마리, 2에 10마리...)
    int32 ZombieCount = Stage * 5;

    for (int32 i = 0; i < ZombieCount; i++)
    {
        FNavLocation SpawnLocation;
        // 반경 2000 유닛 내 랜덤 위치 찾기
        if (NavSys->GetRandomReachablePointInRadius(FVector::ZeroVector, 2000.f, SpawnLocation))
        {
            GetWorld()->SpawnActor<AZombie>(ZombieClass, SpawnLocation.Location, FRotator::ZeroRotator);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Spawned %d zombies for Stage %d"), ZombieCount, Stage);
}

void ASpartaGameMode::ClearAllZombies()
{
    for (TActorIterator<AZombie> It(GetWorld()); It; ++It)
    {
        AZombie* Zombie = *It;
        if (Zombie)
        {
            Zombie->Destroy();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("모든 좀비가 제거되었습니다."));
}
void ASpartaGameMode::StartRandomZombieSpawn()
{
    SpawnZombieRandomly();
}

void ASpartaGameMode::SpawnZombieRandomly()
{
    SpawnZombie();
    float MinDelay = FMath::Max(1.0f, 3.0f - (CurrentStage * 0.5f));
    float MaxDelay = FMath::Max(2.0f, 7.0f - (CurrentStage * 1.0f));
    float RandomDelay = FMath::FRandRange(MinDelay, MaxDelay);

    // 3. 자기 자신을 RandomDelay 초 후에 다시 호출하도록 예약 (단발성)
    GetWorld()->GetTimerManager().SetTimer(
        ZombieSpawnTimerHandle,
        this,
        &ASpartaGameMode::SpawnZombieRandomly,
        RandomDelay,
        false
    );

    UE_LOG(LogTemp, Log, TEXT("Next zombie will spawn in %.2f seconds"), RandomDelay);
}