// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

#include "Components/BoxComponent.h"
#include <Mayday/Characters/CockpitPilot.h>


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

        InteractionBox->OnClicked.AddDynamic(this, &AInteractable::OnInteractionBoxClicked);
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
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        ACockpitPilot* Pilot = Cast<ACockpitPilot>(PC->GetPawn());
        if (Pilot && Pilot->GetIsZoomedIn()) return; // don't highlight while zoomed in
    }

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

void AInteractable::OnInteractionBoxClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
    if (ButtonPressed != EKeys::LeftMouseButton || !CameraMoveLocation) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    ACockpitPilot* Pilot = Cast<ACockpitPilot>(PC->GetPawn());
    if (Pilot)
    {
        Pilot->MoveToLocation(CameraMoveLocation);

        //turn off interact effect as a temp solution
        TArray<UMeshComponent*> MeshComponents;
        GetComponents<UMeshComponent>(MeshComponents);
        for (UMeshComponent* Mesh : MeshComponents)
        {
            Mesh->SetRenderCustomDepth(false);
        }
    }
}

