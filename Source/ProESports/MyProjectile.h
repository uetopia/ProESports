// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Engine.h"
#include "MyPlayerController.h"
#include "MyProjectile.generated.h"

UCLASS()
class PROESPORTS_API AMyProjectile : public AStaticMeshActor
{
	GENERATED_BODY()

		UFUNCTION(Category = "UETOPIA", Server, Reliable, WithValidation)
		virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);



private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UProjectileMovementComponent* MovementComp;
	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystemComponent* ParticleComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UParticleSystem* MyParticleSystem;
	UPROPERTY(EditAnywhere)
	class UMaterial* Material;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* MaterialInstance;


public:

	/** point light component */
	UPROPERTY(VisibleAnywhere, Category = "UETOPIA")
	class UPointLightComponent* PointLight1;

	/** sphere component */
	UPROPERTY(VisibleAnywhere, Category = "UETOPIA")
	class USphereComponent* Sphere1;

	// We want to keep track of who created the bomb so they get credit for any kills
	int32 ownerPlayerID;

	// also keep track of the team ID so teammates don't kill each other
	int32 ownerTeamID;

	//class AMyPlayerController ownerPlayerController;

	// Sets default values for this actor's properties
	AMyProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void setPlayerID(int32 playerID);
	void setTeamID(int32 teamID);
	//void setPlayerController(class AMyPlayerController ownerPlayerController);

	/** the desired intensity for the light */
	UPROPERTY(VisibleAnywhere, Category = "UETOPIA")
		float DesiredIntensity;



};
