#include "CockpitPilot.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Math/UnrealMathUtility.h"

ACockpitPilot::ACockpitPilot()
{
    PrimaryActorTick.bCanEverTick = false;

    CockpitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CockpitRoot"));
    SetRootComponent(CockpitRoot);

    YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
    YawPivot->SetupAttachment(CockpitRoot);

    PitchPivot = CreateDefaultSubobject<USceneComponent>(TEXT("PitchPivot"));
    PitchPivot->SetupAttachment(YawPivot);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(PitchPivot);

    AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void ACockpitPilot::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(CockpitMappingContext, 0);
        }
    }
}

void ACockpitPilot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!ensure(EnhancedInput)) return;

    EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACockpitPilot::Input_Look);
    EnhancedInput->BindAction(IA_FreeLook, ETriggerEvent::Started, this, &ACockpitPilot::Input_FreeLookPressed);
    EnhancedInput->BindAction(IA_FreeLook, ETriggerEvent::Completed, this, &ACockpitPilot::Input_FreeLookReleased);
}

void ACockpitPilot::Input_Look(const FInputActionValue& Value)
{
    if (!bFreeLookHeld || bLookLocked) return;

    const FVector2D Delta = Value.Get<FVector2D>();
    CurrentYaw = FMath::Clamp(CurrentYaw + Delta.X * LookSensitivity, -YawLimit, YawLimit);
    CurrentPitch = FMath::Clamp(CurrentPitch + Delta.Y * LookSensitivity, PitchMin, PitchMax);

    YawPivot->SetRelativeRotation(FRotator(0.f, CurrentYaw, 0.f));
    PitchPivot->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

void ACockpitPilot::Input_FreeLookPressed()
{
    if (!bLookLocked)
    {
        bFreeLookHeld = true;
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
}

void ACockpitPilot::Input_FreeLookReleased()
{
    bFreeLookHeld = false;
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->SetInputMode(FInputModeGameAndUI());
    }
}

void ACockpitPilot::SetLookLocked(bool bLocked)
{
    bLookLocked = bLocked;
    if (bLocked)
    {
        bFreeLookHeld = false;
    }
}