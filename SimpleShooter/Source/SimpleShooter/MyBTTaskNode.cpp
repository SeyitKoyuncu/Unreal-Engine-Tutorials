// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTaskNode.h"
#include "AIController.h"
#include "ShooterCharacter.h"


UMyBTTaskNode::UMyBTTaskNode() 
{
    NodeName = "Shoot";
}

EBTNodeResult::Type UMyBTTaskNode::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Character == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    Character->Shoot();

    return EBTNodeResult::Succeeded;
}
