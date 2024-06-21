// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "SimpleShooter/Characters/ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner()) { return EBTNodeResult::Failed; }

	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!Character) { return EBTNodeResult::Failed; }

	Character->Shoot();
	return EBTNodeResult::Succeeded;
}
