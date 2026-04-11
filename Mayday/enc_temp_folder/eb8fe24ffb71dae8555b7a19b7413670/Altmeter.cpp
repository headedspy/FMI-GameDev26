// Fill out your copyright notice in the Description page of Project Settings.


#include "Altmeter.h"

#include "Components/SceneComponent.h"

AAltmeter::AAltmeter()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneRoot;

    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootBox"));
    Box->SetupAttachment(SceneRoot);

    CutoutMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CutoutMesh"));
    CutoutMesh->SetupAttachment(Box);

    RotorOnes = CreateDefaultSubobject<USceneComponent>(TEXT("RotorOnes"));
    RotorTens = CreateDefaultSubobject<USceneComponent>(TEXT("RotorTens"));
    RotorHundreds = CreateDefaultSubobject<USceneComponent>(TEXT("RotorHundreds"));
    RotorThousands = CreateDefaultSubobject<USceneComponent>(TEXT("RotorThousands"));
    RotorTenthousands = CreateDefaultSubobject<USceneComponent>(TEXT("RotorTenthousands"));

    RotorOnes->SetupAttachment(CutoutMesh);
    RotorTens->SetupAttachment(CutoutMesh);
    RotorHundreds->SetupAttachment(CutoutMesh);
    RotorThousands->SetupAttachment(CutoutMesh);
    RotorTenthousands->SetupAttachment(CutoutMesh);

    PrimaryActorTick.bCanEverTick = true;
}

void AAltmeter::SetInstrumentValue(float InValue)
{
	Super::SetInstrumentValue(InValue);

    int32 AltitudeInt = FMath::Clamp(FMath::RoundToInt(InValue), 0, 99999);

    const float DegreesPerDigit = 36.0f;

    TargetOnes = -(AltitudeInt % 10) * DegreesPerDigit;
    TargetTens = -((AltitudeInt / 10) % 10) * DegreesPerDigit;
    TargetHundreds = -((AltitudeInt / 100) % 10) * DegreesPerDigit;
    TargetThousands = -((AltitudeInt / 1000) % 10) * DegreesPerDigit;
    TargetTenThousands = -((AltitudeInt / 10000) % 10) * DegreesPerDigit;
}

void AAltmeter::BeginPlay()
{
    Super::BeginPlay();
}

void AAltmeter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto InterpRotor = [&](USceneComponent* Rotor, float& Current, float Target)
    {
        Current = FMath::FInterpTo(Current, Target, DeltaTime, RotorInterpSpeed);
        Rotor->SetRelativeRotation(FRotator(0.0f, 0.0f, Current));
    };

    InterpRotor(RotorOnes, CurrentRollOnes, TargetOnes);
    InterpRotor(RotorTens, CurrentRollTens, TargetTens);
    InterpRotor(RotorHundreds, CurrentRollHundreds, TargetHundreds);
    InterpRotor(RotorThousands, CurrentRollThousands, TargetThousands);
    InterpRotor(RotorTenthousands, CurrentRollTenThousands, TargetTenThousands);
}
