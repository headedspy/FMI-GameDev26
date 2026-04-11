// Fill out your copyright notice in the Description page of Project Settings.

#include "Cockpit.h"

#include "Components/ChildActorComponent.h"
#include "Mayday/Instruments/Altmeter.h"
#include "Mayday/Instruments/Airspeed.h"
#include "Mayday/Instruments/VerticalAirspeed.h"
#include "Mayday/Instruments/Heading.h"
#include "Mayday/Instruments/Fuel.h"
#include "Mayday/Modules/Module.h"


ACockpit::ACockpit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACockpit::BeginPlay()
{
	Super::BeginPlay();

    TArray<UChildActorComponent*> Components;
    GetComponents<UChildActorComponent>(Components);

    for (UChildActorComponent* Component : Components)
    {
        if (IsValid(Component) == false)
        {
            UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(Component) == false"));
            continue;
        }

        AActor* Child = Component->GetChildActor();
        if (IsValid(Child) == false)
        {
            UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(Child) == false"));
            continue;
        }

        if (AAltmeter* AltmeterCast = Cast<AAltmeter>(Child)) AltmeterInstrument = AltmeterCast;
        if (AAirspeed* AirspeedCast = Cast<AAirspeed>(Child)) AirspeedInstrument = AirspeedCast;
        if (AVerticalAirspeed* VerticalAirspeedCast = Cast<AVerticalAirspeed>(Child)) VerticalAirspeedInstrument = VerticalAirspeedCast;
        if (AHeading* HeadingCast = Cast<AHeading>(Child)) HeadingInstrument = HeadingCast;
        if (AFuel* FuelCast = Cast<AFuel>(Child)) FuelInstrument = FuelCast;

        if (AModule* ModuleCast = Cast<AModule>(Child)) Modules.Add(ModuleCast);
    }

    if (IsValid(AltmeterInstrument) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(AltmeterInstrument) == false"));
        return;
    }
    if (IsValid(AirspeedInstrument) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(AirspeedInstrument) == false"));
        return;
    }
    if (IsValid(VerticalAirspeedInstrument) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(VerticalAirspeedInstrument) == false"));
        return;
    }
    if (IsValid(HeadingInstrument) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(HeadingInstrument) == false"));
        return;
    }
    if (IsValid(FuelInstrument) == false)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay IsValid(FuelInstrument) == false"));
        return;
    }
    if (Modules.Num() < 5)
    {
        UE_LOG(LogTemp, Error, TEXT("ACockpit::BeginPlay Modules.Num() < 5"));
        return;
    }
	
}

void ACockpit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AltmeterInstrument->SetInstrumentValue(GetActorLocation().Z);
}

