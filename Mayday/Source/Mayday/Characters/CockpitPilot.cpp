#include "CockpitPilot.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Math/UnrealMathUtility.h"

ACockpitPilot::ACockpitPilot()
{
    PrimaryActorTick.bCanEverTick = true; // needed for camera lerp
    PrimaryActorTick.bStartWithTickEnabled = false; // only active during movement

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
        PlayerController->bEnableMouseOverEvents = true;
        PlayerController->bEnableClickEvents = true;

        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(CockpitMappingContext, 0);
        }
    }
}

void ACockpitPilot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bIsMovingCamera) return;

    CameraMoveElapsed += DeltaTime;
    const float Alpha = FMath::Clamp(CameraMoveElapsed / CameraMoveDuration, 0.f, 1.f);
    const float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 2.f);

    // move and rotate the root
    CockpitRoot->SetWorldLocation(FMath::Lerp(LerpFromLocation, LerpToLocation, EasedAlpha));
    CockpitRoot->SetWorldRotation(FMath::Lerp(LerpFromRotation, LerpToRotation, EasedAlpha));

    // simultaneously lerp the free-look pivots
    CurrentYaw = FMath::Lerp(LerpFromYaw, LerpToYaw, EasedAlpha);
    CurrentPitch = FMath::Lerp(LerpFromPitch, LerpToPitch, EasedAlpha);
    YawPivot->SetRelativeRotation(FRotator(0.f, CurrentYaw, 0.f));
    PitchPivot->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));

    if (Alpha >= 1.f)
    {
        bIsMovingCamera = false;
        SetActorTickEnabled(false);
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
    EnhancedInput->BindAction(IA_Return, ETriggerEvent::Started, this, &ACockpitPilot::ReturnToDefault);
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

void ACockpitPilot::MoveToLocation(USceneComponent* Target)
{
    if (!Target || bIsMovingCamera) return;

    // snapshot the restore point only when coming from a non-zoomed state
    if (!bIsZoomedIn)
    {
        PreZoomLocation = CockpitRoot->GetComponentLocation();
        PreZoomRotation = CockpitRoot->GetComponentRotation();
        PreZoomYaw = CurrentYaw;
        PreZoomPitch = CurrentPitch;
    }

    LerpFromLocation = CockpitRoot->GetComponentLocation();
    LerpFromRotation = CockpitRoot->GetComponentRotation();
    LerpFromYaw = CurrentYaw;
    LerpFromPitch = CurrentPitch;

    LerpToLocation = Target->GetComponentLocation();
    LerpToRotation = Target->GetComponentRotation();
    LerpToYaw = 0.f;
    LerpToPitch = 0.f;

    CameraMoveElapsed = 0.f;
    bIsMovingCamera = true;
    bIsZoomedIn = true;

    SetLookLocked(true);
    SetActorTickEnabled(true);
}

void ACockpitPilot::ReturnToDefault()
{
    if (!bIsZoomedIn) return;

    LerpFromLocation = CockpitRoot->GetComponentLocation();
    LerpFromRotation = CockpitRoot->GetComponentRotation();
    LerpFromYaw = CurrentYaw;
    LerpFromPitch = CurrentPitch;

    LerpToLocation = PreZoomLocation;
    LerpToRotation = PreZoomRotation;
    LerpToYaw = PreZoomYaw;
    LerpToPitch = PreZoomPitch;

    CameraMoveElapsed = 0.f;
    bIsMovingCamera = true;
    bIsZoomedIn = false;

    SetActorTickEnabled(true);
    SetLookLocked(false);
}
