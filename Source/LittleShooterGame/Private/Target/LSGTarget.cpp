// Fill out your copyright notice in the Description page of Project Settings.

#include "Target/LSGTarget.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSGTarget, All, All);

ALSGTarget::ALSGTarget()
{
	PrimaryActorTick.bCanEverTick = true;


    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(SphereRadius);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

    FirstBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("First Body Mesh");
    FirstBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FirstBodyMesh->SetupAttachment(GetRootComponent());

    SecondBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Second Body Mesh");
    SecondBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SecondBodyMesh->SetupAttachment(FirstBodyMesh);
}

void ALSGTarget::BeginPlay()
{
	Super::BeginPlay();
    check(CollisionComponent);
    check(FirstBodyMesh);
    check(SecondBodyMesh);

    LerpFuncType = static_cast<ELerpFunctions>(FMath::RandHelper(static_cast<int32>(ELerpFunctions::Count)));
    SpawnPosition = GetActorLocation();
    GenerateNewStartEndMovementPosition();
}

void ALSGTarget::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RandomRoam(DeltaTime);
}

void ALSGTarget::RandomRoam(float DeltaTime)
{
    static float LerpAlpha = 0.0f;
    float LerpAlphaF = 0.0f;

    LerpAlpha = LerpAlpha + DeltaTime;
    LerpAlpha = FMath::Clamp(LerpAlpha, 0.0f, 1.0f);
    LerpAlphaF = FuncFromLerpAlpha(LerpAlpha);

    FVector NewActorLocation;
    NewActorLocation.X = FMath::Lerp(StartMovementPosition.X, EndMovementPosition.X, LerpAlphaF);
    NewActorLocation.Y = FMath::Lerp(StartMovementPosition.Y, EndMovementPosition.Y, LerpAlphaF);
    NewActorLocation.Z = FMath::Lerp(StartMovementPosition.Z, EndMovementPosition.Z, LerpAlphaF);
    SetActorLocation(NewActorLocation);

    FRotator NewRotator = StartRotator;
    NewRotator.Pitch = FMath::Lerp(StartRotator.Pitch, EndRotator.Pitch, LerpAlphaF);
    NewRotator.Yaw = FMath::Lerp(StartRotator.Yaw, EndRotator.Yaw, LerpAlphaF);    
    FirstBodyMesh->SetWorldRotation(NewRotator);
    SecondBodyMesh->SetRelativeRotation(NewRotator);

    if (GetActorLocation().Equals(EndMovementPosition, 0.1f))
    {
        StartRotator = FirstBodyMesh->GetComponentRotation();
        EndRotator = StartRotator;
        EndRotator.Pitch += 45.0f + 90.0f * static_cast<float>(FMath::RandHelper(4));
        EndRotator.Yaw += 45.0f + 90.0f * static_cast<float>(FMath::RandHelper(4));

        GenerateNewStartEndMovementPosition();

        LerpAlpha = 0.0f;
        LerpFuncType = static_cast<ELerpFunctions>(FMath::RandHelper(static_cast<int32>(ELerpFunctions::Count)));
    }
}

void ALSGTarget::GenerateNewStartEndMovementPosition()
{
    StartMovementPosition = GetActorLocation();

    FVector HalfRange = MovementRange.GetAbs() / 2;
    FVector MinRange = -HalfRange;
    FVector MaxRange = HalfRange;
    EndMovementPosition = FVector(
        SpawnPosition.X + FMath::RandRange(MinRange.X, MaxRange.X),
        SpawnPosition.Y + FMath::RandRange(MinRange.Y, MaxRange.Y),
        SpawnPosition.Z + FMath::RandRange(MinRange.Z, MaxRange.Z)
    );
    //UE_LOG(LogLSGTarget, Display, TEXT("EndMovementPosition : %s"), *EndMovementPosition.ToCompactString());
}

float ALSGTarget::FuncFromLerpAlpha(float LerpAlpha)
{
    switch (LerpFuncType)
    {
    case ELerpFunctions::XPowX:
        return LerpAlpha * LerpAlpha;
        break;
    case ELerpFunctions::XPowXPowX:
        return LerpAlpha * LerpAlpha * LerpAlpha;
        break;
    case ELerpFunctions::SQRTX:
        return !FMath::IsNearlyZero(LerpAlpha) ? FMath::Sqrt(LerpAlpha) : LerpAlpha;
        break;
    case ELerpFunctions::SmoothStep:
        {
            float Temp1 = LerpAlpha * LerpAlpha;
            float Temp2 = 1.0f - (1.0f - LerpAlpha) * (1.0 - LerpAlpha);
            return FMath::Lerp(Temp1, Temp2, LerpAlpha);
        }
        break;
    case ELerpFunctions::Count:
        return LerpAlpha;
        break;
    default:
        return LerpAlpha;
        break;
    }

    return LerpAlpha;
}
