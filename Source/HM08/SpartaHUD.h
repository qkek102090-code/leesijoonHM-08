// SpartaHUD.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpartaHUD.generated.h"

class USpartaHUDWidget;
class UUserWidget;

UCLASS()
class HM08_API ASpartaHUD : public AHUD
{
	GENERATED_BODY()
public:
	ASpartaHUD();
    
    virtual void BeginPlay() override;

    void ShowGameHUD();
    
    void UpdateScore(int32 InScore);

    void UpdateTime(int32 InMinute, int32 InTime);

    void UpdateHP(float InCurrentHP, float InMaxHP);

    void UpdateRound(int32 InRound, int32 MaxRound);

    void ShowGameOverMenu();

    void UpdateZombieCount(int32 Count);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USpartaHUDWidget> HUDWidgetClass;

    UPROPERTY()
    USpartaHUDWidget* HUDWidgetInstance;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY()
    UUserWidget* MenuWidgetInstance;
};
