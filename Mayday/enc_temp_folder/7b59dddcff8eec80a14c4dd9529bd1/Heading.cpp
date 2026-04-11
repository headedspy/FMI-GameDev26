// Fill out your copyright notice in the Description page of Project Settings.


#include "Heading.h"

AHeading::AHeading()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	RotatingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RotatingComponent"));
	RotatingComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
}

void AHeading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto InterpCompass = [&](float& Current, float Target)
	{
		Current = FMath::FInterpTo(Current, Target, DeltaTime, CompassInterpSpeed);
		RotatingComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, Current));
	};

	InterpCompass(CurrentHeading, TargetHeading);
}

void AHeading::SetInstrumentValue(float InValue)
{
	Super::SetInstrumentValue(InValue);

	TargetHeading = InValue;
}
