// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mayday/Misc/Interactable.h"
#include "Instrument.generated.h"

UCLASS()
class MAYDAY_API AInstrument : public AInteractable
{
	GENERATED_BODY()

public:
	AInstrument();

	virtual void SetInstrumentValue(float InValue);

protected:

private:
	UPROPERTY()
	float Value;

	UPROPERTY()
	class AModule* Module;
};
