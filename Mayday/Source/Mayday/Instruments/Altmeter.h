// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Instrument.h"
#include "Altmeter.generated.h"


UCLASS()
class MAYDAY_API AAltmeter : public AInstrument
{
	GENERATED_BODY()

public:
	AAltmeter();

	virtual void SetInstrumentValue(float InValue) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Box;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CutoutMesh;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotorOnes;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotorTens;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotorHundreds;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotorThousands;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* RotorTenthousands;

	// Target roll angles for each rotor
	float TargetOnes;
	float TargetTens;
	float TargetHundreds;
	float TargetThousands;
	float TargetTenThousands;

	float CurrentRollOnes;
	float CurrentRollTens;
	float CurrentRollHundreds;
	float CurrentRollThousands;
	float CurrentRollTenThousands;


	// How fast the rotors spin (degrees per second)
	UPROPERTY(EditAnywhere, Category = "Altimeter")
	float RotorInterpSpeed = 10.0f;
};
