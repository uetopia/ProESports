// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectile.h"
#include "Comp.h"
#include "UEtopiaCompetitiveCharacter.h"
#include "Engine.h"
//For the UGameplayStatics::SpawnEmitterAttached
#include "MyPlayerState.h"
#include "ParticleDefinitions.h"



// Sets default values
AMyProjectile::AMyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh'/Game/Geometry/Meshes/1M_Cube_Socket.1M_Cube_Socket'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetStaticMesh(SphereVisualAsset.Object);
	GetStaticMeshComponent()->SetRelativeLocation(FVector(1.0f, 1.0f, -3.0f));
	GetStaticMeshComponent()->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	//RootComponent = StaticMeshComponent;

	if (!IsRunningDedicatedServer()) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyGameState] LoadLevel - Not a Dedicated server."));
		// For some reason the dedicated server can't load the material, but it does not need it.

		static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("Material'/Game/m_glow.m_glow'"));
		// Material'/Game/m_glow.m_glow'
		if (MatFinder.Succeeded())
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyServerPortalActor] [Construct]  "));
			Material = MatFinder.Object;
			MaterialInstance = UMaterialInstanceDynamic::Create(Material, NULL);
			GetStaticMeshComponent()->SetMaterial(0, MaterialInstance);
		}


	}

	DesiredIntensity = 30000.0f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	//CollisionComp->OnComponentBeginOverlap.Add(*OnHit);
	RootComponent = CollisionComp;

	GetStaticMeshComponent()->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->MaxSpeed = 1500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;
	MovementComp->bShouldBounce = true;
	MovementComp->Bounciness = 100.f;
	MovementComp->Friction = 0.f;

	// ParticleSystem'/Game/p_progectile_effect.p_progectile_effect'
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PSClass(TEXT("ParticleSystem'/Game/p_projectile_effect.p_projectile_effect'"));

	if (PSClass.Object != NULL) {
		MyParticleSystem = PSClass.Object; // MyParticleSystem is a UParticleSystem pointer
	}










	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();


	UGameplayStatics::SpawnEmitterAttached(
		MyParticleSystem,                   //particle system
		GetStaticMeshComponent(),      //mesh to attach to
		FName("Head"),   //socket name
		FVector(0, 0, 64),  //location relative to socket
		FRotator(0, 0, 0), //rotation
		EAttachLocation::KeepRelativeOffset,
		true //will be deleted automatically
		);


}

// Called every frame
void AMyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyProjectile::setPlayerID(int32 playerID)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [setPlayerID] %i "), playerID);
	ownerPlayerID = playerID;
}

void AMyProjectile::setTeamID(int32 teamID)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [setTeamID] %i "), teamID);
	ownerTeamID = teamID;
}

//void AMyBomb::setPlayerController(AMyPlayerController playerController)
//{
//	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [setPlayerController] "));
//	ownerPlayerController = playerController*;
//}

void AMyProjectile::OnHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [OnHit] "));
	// cast to our chatacter.


	AUEtopiaCompetitiveCharacter* TheCharacter = Cast<AUEtopiaCompetitiveCharacter>(OtherActor);
	if (TheCharacter) {
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [OnHit] Cast to UEtopiaCompetitiveCharacter"));

		// check team?  Friendly Fire.

		AMyPlayerState* playerS = Cast<AMyPlayerState>(TheCharacter->PlayerState);

		if (playerS->TeamId != ownerTeamID)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [OnHit] Found a character on a different team - causing damage"));

			// Create a damage event  
			TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);


			TheCharacter->TakeDamageCustom(.3f, DamageEvent, this);
		}

		



	}
	else {
		// It wasn't a character hit.  Just destroy it.
		bool destroyed = Destroy();
	}

	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyBomb] [OnHit] OtherActor: %s"), OtherActor.);
}

bool AMyProjectile::OnHit_Validate(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	return true;
}