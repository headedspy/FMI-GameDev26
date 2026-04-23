// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class MAYDAY_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:

	AInteractable();

	UFUNCTION(BlueprintCallable)
	void SetupDynamics();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* InteractionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* CameraMoveLocation;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	bool IsActive = false;

	UFUNCTION()
	void OnInteractionBoxBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnInteractionBoxEndCursorOver(UPrimitiveComponent* TouchedComponent);


	UFUNCTION()
	void OnInteractionBoxClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

};
