// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSGBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class LITTLESHOOTERGAME_API ALSGBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSGBaseWeapon();

	void PickUp(USceneComponent* PickedUpByComponent, FName SocketName);
	bool CanBePickedUp();
	void Drop();
	void PunchIt(FVector Impulse);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName CoreBoneName = "CoreBoneName";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0", ClampMax = "1000"))
	int32 AmmoMax = 30;

	int32 Ammo = AmmoMax;

	bool IsPickedUp = false;



	virtual void BeginPlay() override;

};
