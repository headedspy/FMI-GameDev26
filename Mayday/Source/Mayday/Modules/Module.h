// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Module.generated.h"

UCLASS()
class MAYDAY_API AModule : public AActor
{
	GENERATED_BODY()
	
public:
	AModule();

	void Activate();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	bool IsActive;

	UPROPERTY()
	class AInstrument* Instrument;

};
