// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CockpitPilot.generated.h"


UCLASS()
class MAYDAY_API ACockpitPilot : public APawn
{
    GENERATED_BODY()

public:
    ACockpitPilot();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void SetLookLocked(bool bLocked);

    void MoveToLocation(USceneComponent* Target);

    void ReturnToDefault();

    FORCEINLINE bool GetIsZoomedIn() { return bIsZoomedIn; }

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit|Components")
    TObjectPtr<USceneComponent> CockpitRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit|Components")
    TObjectPtr<USceneComponent> YawPivot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit|Components")
    TObjectPtr<USceneComponent> PitchPivot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit|Components")
    TObjectPtr<class UCameraComponent> Camera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cockpit|Input")
    TObjectPtr<class UInputMappingContext> CockpitMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cockpit|Input")
    TObjectPtr<class UInputAction> IA_Look;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cockpit|Input")
    TObjectPtr<class UInputAction> IA_FreeLook;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cockpit|Input")
    TObjectPtr<UInputAction> IA_Return;

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float LookSensitivity = 0.6f;

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float YawLimit = 110.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float PitchMin = -40.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float PitchMax = 60.0f;

private:
    void Input_Look(const struct FInputActionValue& Value);
    void Input_FreeLookPressed();
    void Input_FreeLookReleased();

    float CurrentYaw = 0.0f;
    float CurrentPitch = 0.0f;
    bool  bFreeLookHeld = false;
    bool  bLookLocked = false;

    // from
    FVector  LerpFromLocation = FVector::ZeroVector;
    FRotator LerpFromRotation = FRotator::ZeroRotator;
    float    LerpFromYaw = 0.f;
    float    LerpFromPitch = 0.f;

    // to
    FVector  LerpToLocation = FVector::ZeroVector;
    FRotator LerpToRotation = FRotator::ZeroRotator;
    float    LerpToYaw = 0.f;
    float    LerpToPitch = 0.f;

    // snapshot
    FVector  PreZoomLocation = FVector::ZeroVector;
    FRotator PreZoomRotation = FRotator::ZeroRotator;
    float    PreZoomYaw = 0.f;
    float    PreZoomPitch = 0.f;

    bool  bIsMovingCamera = false;
    bool  bIsZoomedIn = false;
    float CameraMoveDuration = 0.3f;
    float CameraMoveElapsed = 0.f;
};