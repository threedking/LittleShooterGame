// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSGBaseWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/LSGProjectile.h"
#include "DrawDebugHelpers.h"

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
	check(ProjectileClass);
}

void ALSGBaseWeapon::PickUp(USceneComponent* PickedUpByComponent, FName SocketName)
{
	if (!IsPickedUp)
	{
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachToComponent(PickedUpByComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true), SocketName);
		CurrentOwner = PickedUpByComponent->GetOwner(); 
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
		CurrentOwner = nullptr;
		IsPickedUp = false;
	}
}

void ALSGBaseWeapon::PunchIt(FVector Impulse)
{
	WeaponMesh->AddImpulse(Impulse, CoreBoneName);
}

void ALSGBaseWeapon::MakeOneShot(FVector TargetLocation)
{
	if(!GetWorld() || !IsCanShot())
	{
		return;
	}

	auto MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	//DrawDebugSphere(GetWorld(), MuzzleLocation, 10, 8, FColor::Red, false, 5.0f);
	auto ShotDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
	
	ALSGProjectile* Projectile = this->GetWorld()->SpawnActorDeferred<ALSGProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShotDirection(ShotDirection);
		Projectile->SetOwner(CurrentOwner);
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
}

bool ALSGBaseWeapon::IsCanShot()
{
	return Ammo > 0;
}

void ALSGBaseWeapon::DecreaseAmmo()
{
	if (Ammo > 0) 
	{
		Ammo--;
	}
}

bool ALSGBaseWeapon::TryReload()
{
	if (Ammo < AmmoMax)
	{
		Ammo = AmmoMax;
		return true;
	}
	return false;
}

int32 ALSGBaseWeapon::GetAmmo() const
{
	return Ammo;
}

