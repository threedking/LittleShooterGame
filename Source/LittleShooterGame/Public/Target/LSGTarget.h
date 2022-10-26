// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSGTarget.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UENUM()
enum class ELerpFunctions : uint8
{
	XPowX = 0U,
	XPowXPowX,
	SQRTX,
	SmoothStep,
	Count
};

UCLASS()
class LITTLESHOOTERGAME_API ALSGTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSGTarget();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* FirstBodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* SecondBodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default Params")
	float SphereRadius = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default Params")
	FVector MovementRange;

	ELerpFunctions LerpFuncType;
	FVector SpawnPosition;
	FVector StartMovementPosition;
	FVector EndMovementPosition;
	FRotator StartRotator;
	FRotator EndRotator;

	virtual void BeginPlay() override;

	void RandomRoam(float DeltaTime);
	void GenerateNewStartEndMovementPosition();
	float FuncFromLerpAlpha(float LerpAlpha);

public:	
	virtual void Tick(float DeltaTime) override;

};
