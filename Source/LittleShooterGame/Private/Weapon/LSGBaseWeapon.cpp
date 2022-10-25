// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSGBaseWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ALSGBaseWeapon::ALSGBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}

void ALSGBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
}

void ALSGBaseWeapon::PickUp(USceneComponent* PickedUpByComponent, FName SocketName)
{
	if (!IsPickedUp)
	{
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachToComponent(PickedUpByComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), SocketName);
		IsPickedUp = true;
	}
}

bool ALSGBaseWeapon::CanBePickedUp()
{
	return !IsPickedUp;
}

void ALSGBaseWeapon::Drop()
{
	if (IsPickedUp)
	{
		DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		IsPickedUp = false;
	}
}

void ALSGBaseWeapon::PunchIt(FVector Impulse)
{
	WeaponMesh->AddImpulse(Impulse, CoreBoneName);
}