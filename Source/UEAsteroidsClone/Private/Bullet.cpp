// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "ScreenWarper.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet() :
	LifeTime(3.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	RootComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BulletMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	ScreenWarperComp = CreateDefaultSubobject<UScreenWarper>(TEXT("Screen Warper"));

}

void ABullet::InitializeBullet(FVector Velocity, UMaterialInterface* BulletMat)
{
	SetLifeSpan(LifeTime);

	ProjectileMovementComponent->Velocity = Velocity;

	// Changed bullet material if BulletMat is not null
	if(BulletMat)
	{
		BulletMesh->SetMaterial(0,BulletMat);
	}
	
}


void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this && OtherActor != GetOwner())
	{		
		OtherActor->TakeDamage(0,FDamageEvent(UDamageType::StaticClass()),UGameplayStatics::GetPlayerController(this, 0),this);
		Destroy();
	}
}


