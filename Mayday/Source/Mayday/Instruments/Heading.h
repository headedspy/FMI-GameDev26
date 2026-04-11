// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Instrument.h"
#include "Heading.generated.h"


UCLASS()
class MAYDAY_API AHeading : public AInstrument
{
	GENERATED_BODY()

public:
	AHeading();

	virtual void Tick(float DeltaTime) override;

	virtual void SetInstrumentValue(float InValue) override;

private:

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RootSceneComponent;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotatingComponent;

	float TargetHeading;

	float CurrentHeading;

	UPROPERTY(EditAnywhere)
	float CompassInterpSpeed = 25.0f;
};
