// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSGProjectile.h"
#include "Components/SphereComponent.h"
#include "LittleShooterGameCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALSGProjectile::ALSGProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Sphere Component");
    SetRootComponent(CollisionComponent);
    CollisionComponent->InitSphereRadius(5);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement Component");
    MovementComponent->ProjectileGravityScale = 0.0f;
    
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    ProjectileMesh->SetupAttachment(CollisionComponent);

    bGenerateOverlapEventsDuringLevelStreaming = true;
}

void ALSGProjectile::BeginPlay()
{
	Super::BeginPlay();
    check(CollisionComponent);
	check(MovementComponent);
	check(ProjectileMesh);
    checkf(LifeSpan > 0.1f, TEXT("LifeSpan too small"));
	
    if (GetOwner()) 
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    }
    
    CollisionComponent->OnComponentHit.AddDynamic(this, &ALSGProjectile::OnProjectileHit);

    SetLifeSpan(LifeSpan);
}

void ALSGProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALSGProjectile::SetShotDirection(FVector ShotDirection)
{
	MovementComponent->Velocity = ShotDirection.GetSafeNormal();
    ProjectileMesh->SetWorldRotation(ShotDirection.Rotation());
}

void ALSGProjectile::SetInitialSpeed(float InitialSpeed)
{
	MovementComponent->InitialSpeed = InitialSpeed;
}


void ALSGProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!this->GetWorld())
    {
        return;
    }

    MovementComponent->StopMovementImmediately();

    AController* OwnerController = nullptr;

    if (GetOwner())
    {
        auto LSGCharacter = Cast<ALittleShooterGameCharacter>(GetOwner());
        if (LSGCharacter)
        {
            LSGCharacter->VictimDamagedNotify(OtherActor);
            OwnerController = LSGCharacter->GetController();            
        }
    }

    OtherActor->TakeDamage(DamageAmount, FDamageEvent{}, OwnerController, this);
    Destroy();
}