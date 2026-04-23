// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cockpit.generated.h"

UCLASS()
class MAYDAY_API ACockpit : public AActor
{
	GENERATED_BODY()
	
public:
	ACockpit();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	float Altitude;

	UPROPERTY()
	float Airspeed;

	UPROPERTY()
	float VerticalAirspeed;

	UPROPERTY()
	float Heading;

	UPROPERTY()
	float Fuel;

	UPROPERTY()
	bool IsGearDown;


	UPROPERTY(EditDefaultsOnly)
	class AAltmeter* AltmeterInstrument;

	UPROPERTY(EditDefaultsOnly)
	class AAirspeed* AirspeedInstrument;

	UPROPERTY(EditDefaultsOnly)
	class AVerticalAirspeed* VerticalAirspeedInstrument;

	UPROPERTY(EditDefaultsOnly)
	class AHeading* HeadingInstrument;

	UPROPERTY(EditDefaultsOnly)
	class AFuel* FuelInstrument;

	UPROPERTY(EditDefaultsOnly)
	TArray<class AModule*> Modules;

	UPROPERTY(EditDefaultsOnly)
	class AYoke* Yoke;
};
