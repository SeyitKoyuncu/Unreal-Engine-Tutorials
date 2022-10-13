// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay(); 

    if(AIBehavior)
    {
        RunBehaviorTree(AIBehavior);

        APawn * PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // return the main controlled chrachter pawn
        GetBlackboardComponent() -> SetValueAsVector(TEXT("StartLocation"), GetPawn() -> GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    /*
    APawn * PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // return the main controlled chrachter pawn

    // IF LineOfSight   
        // MoveTo
        // SetFocus
    
    //else
        // Clear Foucs
        // stop movement

    if(LineOfSightTo(PlayerPawn))
    {
        SetFocus(PlayerPawn); // with this, other actor follow the main chrachter and rotate his to the main chrachter
        MoveToActor(PlayerPawn, AcceptanceRadius); 
    }

    else
    {
        ClearFocus(EAIFocusPriority::Gameplay); //stop the ai rotation foucus   
        StopMovement(); //stop the ai movement
    }
    */


}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}
