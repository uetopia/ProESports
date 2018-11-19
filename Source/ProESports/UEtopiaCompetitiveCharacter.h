// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MyTypes.h"
#include "UEtopiaCompetitiveCharacter.generated.h"

UCLASS(config = Game)
class AUEtopiaCompetitiveCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;



	/** player pressed start fire action */
	void OnStartFire();

	/** player released start fire action */
	void OnStopFire();

public:
	AUEtopiaCompetitiveCharacter();

	// BP implementable functions seem to be more reliable in some cases than delegates, so we're using them here.
	UFUNCTION(Client, Reliable)
		void ClientChangeUIState(EConnectUIState NewState);
	UFUNCTION(BlueprintNativeEvent)
		void OnUIStateChange(EConnectUIState UIState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		TArray<FCharacterOptions> character_options;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OnAppearanceChange, Category = "UETOPIA")
		FMyAppearance MyAppearance;

	UFUNCTION()
		void OnRep_OnAppearanceChange();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void EquipAppearance();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** UETOPIA ADDED VARS. */
	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "UETOPIA")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = " UETOPIA")
		int32 ProjectileCount;

	//moved to player state
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = " UETOPIA")
	//	int32 TeamId;


	/** Take damage, handle death */
	virtual float TakeDamageCustom(float Damage, struct FDamageEvent const& DamageEvent, class AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		bool hasProjectiles();

	// This is the function that gets called in the widget Blueprint
	UFUNCTION(BlueprintCallable, Category = "UETOPIA", Server, Reliable, WithValidation)
		void ServerAttemptSpawnProjectile();
	// valid/imp methods

	/**
	* Kills pawn.  Server/authority only.
	* @param KillingDamage - Damage amount of the killing blow
	* @param DamageEvent - Damage event of the killing blow
	* @param Killer - Who killed this pawn
	* @param DamageCauser - the Actor that directly caused the damage (i.e. the Projectile that exploded, the Weapon that fired, etc)
	* @returns true if allowed
	*/
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AActor* DamageCauser);

	/** Text Render */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UTextRenderComponent* Text;

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


	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** check if pawn is still alive */
	bool IsAlive() const;

	/** check if pawn can fire weapon */
	bool CanFire() const;

	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

