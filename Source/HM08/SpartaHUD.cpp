// SpartaHUD.cpp


#include "SpartaHUD.h"
#include "SpartaHUDWidget.h"

ASpartaHUD::ASpartaHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> FoundHUDWidgetClass(TEXT("/Game/WBP_SpartaHUD.WBP_SpartaHUD_C"));

	if (FoundHUDWidgetClass.Succeeded() == true)
	{
		HUDWidgetClass = FoundHUDWidgetClass.Class;
	}
    ConstructorHelpers::FClassFinder<UUserWidget> FoundMenuWidgetClass(TEXT("/Game/WBP_Menu.WBP_Menu_C"));
    // /Script/UMGEditor.WidgetBlueprint'/Game/WBP_Menu.WBP_Menu'
    if (FoundMenuWidgetClass.Succeeded() == true)
    {
        MenuWidgetClass = FoundMenuWidgetClass.Class;
    }
}

void ASpartaHUD::BeginPlay()
{
    Super::BeginPlay();

    ShowGameHUD();
}

void ASpartaHUD::ShowGameHUD()
{
    if (HUDWidgetClass != nullptr && HUDWidgetInstance == nullptr)
    {
        HUDWidgetInstance = CreateWidget<USpartaHUDWidget>(GetWorld(), HUDWidgetClass);
    }

    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->AddToViewport();
    }
}

void ASpartaHUD::UpdateRound(int32 InRound, int32 MaxRound)
{
    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->SetRound(InRound, MaxRound);
    }
}

void ASpartaHUD::UpdateScore(int32 InScore)
{
    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->SetScore(InScore);
    }
}

void ASpartaHUD::UpdateTime(int32 InMinute, int32 InTime)
{
    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->SetTime(InMinute, InTime);
    }
}

void ASpartaHUD::UpdateHP(float InCurrentHP, float InMaxHP)
{
    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->SetHP(InCurrentHP, InMaxHP);
    }
}

void ASpartaHUD::ShowGameOverMenu()
{
    if (MenuWidgetClass != nullptr && MenuWidgetInstance == nullptr)
    {
        MenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    }

    if (MenuWidgetInstance != nullptr)
    {
        MenuWidgetInstance->AddToViewport();
    }
}

void ASpartaHUD::UpdateZombieCount(int32 Count)
{
    if (HUDWidgetInstance != nullptr)
    {
        HUDWidgetInstance->SetZombie(Count);
    }
}
