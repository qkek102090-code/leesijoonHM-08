// SpartaHUDWidget.cpp

#include "SpartaHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void USpartaHUDWidget::SetScore(int32 InScore)
{
    if (ScoreText != nullptr)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), InScore)));
    }
}

void USpartaHUDWidget::SetTime(int32 InMinute, int32 InTime)
{
    if (TimeText != nullptr)
    {
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"),InMinute, InTime)));
    }
}

void USpartaHUDWidget::SetHP(float InCurrentHP, float InMaxHP)
{
    if (HPBar != nullptr && InMaxHP > 0.f)
    {
        HPBar->SetPercent(InCurrentHP / InMaxHP);
    }
}

void USpartaHUDWidget::SetRound(int32 InRound, int32 MaxRound)
{
    if (RoundText != nullptr)
    {
        RoundText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), InRound, MaxRound)));
    }
}

void USpartaHUDWidget::SetZombie(int32 Count)
{
    if (CountText != nullptr)
    {
        CountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
    }
}
