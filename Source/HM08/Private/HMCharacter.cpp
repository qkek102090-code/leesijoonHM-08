// HMCharacter.cpp
#include "HMCharacter.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "HM08/SpartaHUD.h"
#include "HM08/SpartaGameMode.h"
AHMCharacter::AHMCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 0.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AHMCharacter::BeginPlay()
{
	Super::BeginPlay();
    SyncHUD();
}

void AHMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHMCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHMCharacter::StartJump
                );
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AHMCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHMCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHMCharacter::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AHMCharacter::StopSprint
                );
            }
        }
    }
}

void AHMCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AHMCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void AHMCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AHMCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AHMCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AHMCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}
void AHMCharacter::SyncHUD()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ASpartaHUD* SpartaHUD = Cast<ASpartaHUD>(PC->GetHUD()))
        {
            SpartaHUD->UpdateHP(Health, MaxHealth);
        }
    }
}

void AHMCharacter::UpdateHP(float Amount)
{
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

    SyncHUD();

    if (Health <= 0.0f)
    {
        HandleDeath();
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC != nullptr)
        {
            ASpartaHUD* SpartaHUD = Cast<ASpartaHUD>(PC->GetHUD());
            if (SpartaHUD != nullptr)
            {
                SpartaHUD->ShowGameOverMenu();
            }
        }
        UE_LOG(LogTemp, Warning, TEXT("Character is Dead!"));
    }
}
void AHMCharacter::HandleDeath()
{
    UE_LOG(LogTemp, Error, TEXT("플레이어 사망"));

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    DisableInput(PC);

    PC->bShowMouseCursor = true;

    FInputModeUIOnly InputMode;
    PC->SetInputMode(InputMode);

    GetCharacterMovement()->DisableMovement();

    GetMesh()->bPauseAnims = true;

    UGameplayStatics::SetGamePaused(GetWorld(), true);
}