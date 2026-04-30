// SpartaHUDWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpartaHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * */
UCLASS()
class HM08_API USpartaHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetScore(int32 InScore);

    void SetTime(int32 InMinute, int32 InTime);

    void SetHP(float InCurrentHP, float InMaxHP);

    void SetRound(int32 InRound, int32 MaxRound);

    void SetZombie(int32 Count);
protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimeText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RoundText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CountText;
};