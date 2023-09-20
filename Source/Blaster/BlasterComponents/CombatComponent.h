// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void UnequipWeapon(class AWeapon* WeaponToUnequip);

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void DropWeapon(class AWeapon* WeaponToDrop);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipAndSwap(class AWeapon* WeaponToEquip);


	void Reload();
	UFUNCTION(BlueprintCallable)
		void FinishReloading();

	UFUNCTION(BlueprintCallable)
		void FinishSwap();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
		void ShotgunShellReload();

	void JumpToShotgunEnd();

	UFUNCTION(BlueprintCallable)
		void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
		void LaunchGrenade();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
		AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
		AWeapon* SecondaryWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_ThirdWeapon)
		AWeapon* ThirdWeapon;

	void AttachActorToBackpack(AActor* ActorToAttach);
	void AttachActorToBackpack2(AActor* ActorToAttach);
	void AttachActorToBackpack3(AActor* ActorToAttach);

	UFUNCTION(Server, Reliable)
		void ServerLaunchGrenade(const FVector_NetQuantize& Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		EWeaponType WeaponType; // Use EWeaponType here

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AmmoAmount;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		bool PickupAmmo(EWeaponType My_WeaponType, int32 My_AmmoAmount);

	bool bLocalPickupSuccessful = false;
	bool bPickupSuccessful = false;
	bool bLocallyReloading = false;

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
		void OnRep_EquippedWeapon();

	UFUNCTION()
		void OnRep_SecondaryWeapon();

	UFUNCTION()
		void OnRep_ThirdWeapon();

	void Fire();
	void FireProjectileWeapon();
	void FireHitScanWeapon();
	void FireShotgun();
	void LocalFire(const FVector_NetQuantize& TraceHitTarget);
	void ShotgunLocalFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire(const FVector_NetQuantize& TraceHitTarget, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets, float FireDelay);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastShotgunFire(const TArray<FVector_NetQuantize>& TraceHitTargets);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
		void ServerReload();

	void HandleReload();
	int32 AmountToReload();

	void ThrowGrenade();

	UFUNCTION(Server, Reliable)
		void ServerThrowGrenade();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AProjectile> GrenadeClass;

	void DropEquippedWeapon();
	void DropSecondaryWeapon();
	void DropAndEquipWeapon(AWeapon* WeaponToEquip);
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);


	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);
	void ReloadEmptyWeapon();
	void UpdateWeaponIcons();

	void ShowAttachedGrenade(bool bShowGrenade);
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);
	void EquipThirdWeapon(AWeapon* WeaponToEquip);


private:
	UPROPERTY()
		class ABlasterCharacter* Character;
	UPROPERTY()
		class ABlasterPlayerController* Controller;
	UPROPERTY()
		class ABlasterHUD* HUD;


	UPROPERTY(ReplicatedUsing = OnRep_Aiming)
		bool bAiming = false;

	bool bAimButtonPressed = false;

	UFUNCTION()
		void OnRep_Aiming();

	UPROPERTY(EditAnywhere)
		float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
		float AimWalkSpeed;

	bool bFireButtonPressed;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

	FHUDPackage HUDPackage;

	/**
	* Aiming and FOV
	*/

	// Field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/**
	* Automatic fire
	*/

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
		int32 CarriedAmmo;

	UFUNCTION()
		void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
		int32 MaxCarriedAmmo = 400;

	UPROPERTY(EditAnywhere)
		int32 StartingARAmmo = 30;

	UPROPERTY(EditAnywhere)
		int32 StartingRocketAmmo = 0;

	UPROPERTY(EditAnywhere)
		int32 StartingPistolAmmo = 0;

	UPROPERTY(EditAnywhere)
		int32 StartingSMGAmmo = 0;

	UPROPERTY(EditAnywhere)
		int32 StartingShotgunAmmo = 0;

	UPROPERTY(EditAnywhere)
		int32 StartingSniperAmmo = 0;

	UPROPERTY(EditAnywhere)
		int32 StartingGrenadeLauncherAmmo = 0;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
		ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
		void OnRep_CombatState();

	void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();

	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
		int32 Grenades = 4;

	UFUNCTION()
		void OnRep_Grenades();

	UPROPERTY(EditAnywhere)
		int32 MaxGrenades = 4;

	void UpdateHUDGrenades();

public:
	FORCEINLINE int32 GetGrenades() const { return Grenades; }
	AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
};