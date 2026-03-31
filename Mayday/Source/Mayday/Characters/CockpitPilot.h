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

    // Call this to lock/unlock look during reboot procedures
    void SetLookLocked(bool bLocked);

protected:
    virtual void BeginPlay() override;

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

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float LookSensitivity = 0.6f;

    UPROPERTY(EditDefaultsOnly, Category = "Cockpit|Feel")
    float YawLimit = 110.0f;   // degrees either side

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
};