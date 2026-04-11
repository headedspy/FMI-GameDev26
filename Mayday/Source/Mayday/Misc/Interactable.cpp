// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Components/BoxComponent.h"


AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractable::SetupDynamics()
{
    if (InteractionBox)
    {
        InteractionBox->OnBeginCursorOver.AddDynamic(this, &AInteractable::OnInteractionBoxBeginCursorOver);
        InteractionBox->OnEndCursorOver.AddDynamic(this, &AInteractable::OnInteractionBoxEndCursorOver);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: InteractionBox is not assigned!"), *GetName());
    }
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::OnInteractionBoxBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
    TArray<UMeshComponent*> MeshComponents;
    GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* Mesh : MeshComponents)
    {
        Mesh->SetRenderCustomDepth(true);
        Mesh->SetCustomDepthStencilValue(1);
    }
}

void AInteractable::OnInteractionBoxEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
    TArray<UMeshComponent*> MeshComponents;
    GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* Mesh : MeshComponents)
        Mesh->SetRenderCustomDepth(false);
}

