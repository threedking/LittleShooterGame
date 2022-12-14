// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LittleShooterGameCharacter.generated.h"

class ALSGBaseWeapon;

UCLASS(config=Game)
class ALittleShooterGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ALittleShooterGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void VictimDamagedNotify(const AActor* Victim);

	void Shot();
	void Reload();

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	ALSGBaseWeapon* Weapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0", ClampMax = "10000"))
	float WeaponPunchingPower = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0", ClampMax = "1000"))
	int32 ClipsAmount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0", ClampMax = "90"))
	float BulletConeRadiusDeg = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "500", ClampMax = "3000"))
	float TraceMaxDistance = 1000.0f;

	int32 Score = 0;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void PickUpWeapon(ALSGBaseWeapon* LyingWeapon);
	void DropWeapon();

	bool GetAimTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool GetViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHasWeaponChanged(bool HasWeapon);

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	bool HasWeapon() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	int32 GetClipsAmount() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	int32 GetAmmoAmountInCurrentWeapon() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	int32 GetScore() const;

};

