// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Yoke.generated.h"


UCLASS()
class MAYDAY_API AYoke : public AInteractable
{
	GENERATED_BODY()

private:

	UPROPERTY()
	float Roll;
	
	UPROPERTY()
	float Pitch;
};
