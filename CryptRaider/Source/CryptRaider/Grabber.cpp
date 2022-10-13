// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent * PyhsicHandle = GetPhysicsHandle();

	if(PyhsicHandle && PyhsicHandle -> GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PyhsicHandle -> SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}


}

void UGrabber::Release()	
{
	UE_LOG(LogTemp, Display, TEXT("Released grabber"));

	UPhysicsHandleComponent * PyhsicHandle = GetPhysicsHandle();
	if(PyhsicHandle && PyhsicHandle -> GetGrabbedComponent())
	{
		AActor *  GrabbedActor = PyhsicHandle -> GetGrabbedComponent() -> GetOwner();
		GrabbedActor -> Tags.Remove("Grabbed");
		PyhsicHandle -> ReleaseComponent();
	}
}

void UGrabber::Grab()
{

	UPhysicsHandleComponent * PyhsicHandle = GetPhysicsHandle();
	if(PyhsicHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbebleInReach(HitResult);

	if (HasHit)
	{
		UPrimitiveComponent	* HitComponent = HitResult.GetComponent();
		HitComponent -> WakeAllRigidBodies();
		HitResult.GetActor() -> Tags.Add("Grabbed");
		PyhsicHandle -> GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

UPhysicsHandleComponent * UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent * result = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if(result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires UPhysichandle component."));
	}
	return result;
}


bool UGrabber::GetGrabbebleInReach(FHitResult & OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,	
		ECC_GameTraceChannel2,
		Sphere
	);
}
