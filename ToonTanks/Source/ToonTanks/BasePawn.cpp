// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// We creating the capsule component to the BasePawn with this. Like we created in the blueprint editor.
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider")); //Create a component or subobject, allows creating a child class and returning the parent class.
	RootComponent = CapsuleComp; //USceneComponent * RootComponent. The component that defines the transform (location, rotation, scale) of this Actor in the world, all other components must be attached to this one somehow.

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh -> SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh -> SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint -> SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh -> GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	//We could use TurretMesh->SetWorldRotation(LookAtRotation), but this is better because with this turret rotate smoothly
	TurretMesh -> SetWorldRotation(
		FMath::RInterpTo(TurretMesh -> GetComponentRotation(), 
		LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 
		5.f));
}

void ABasePawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint -> GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile -> SetOwner(this); //when projectile spawn we set owner to the projectile and after that we can take this with get owner in the other files.
}

void ABasePawn::HandleDestruction()
{
	// TODO: Visual / Sounds effects
	if (DeathParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());

	if(DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());


	if(DeathCameraShakeClass)
		GetWorld() -> GetFirstPlayerController() -> ClientStartCameraShake(DeathCameraShakeClass); 
}



