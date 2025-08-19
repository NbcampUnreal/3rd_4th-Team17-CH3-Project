// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Projectile/PEProjectileBase.h"

#include "Combat/Components/PEReceiveAttackComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"
#include "Items/Weapons/PEWeaponBase.h"
#include "Kismet/GameplayStatics.h"

APEProjectileBase::APEProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 설정
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetBoxExtent(FVector(2.5f, 2.5f, 5.0f)); // 박스 크기 설정 (길이 5, 폭/높이 2.5)
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetHiddenInGame(true);
	RootComponent = CollisionComponent;

	// 메시 컴포넌트 설정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 투사체 이동 컴포넌트 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f; // 중력 활성화 (실제 총알처럼)

	ProjectileLifetime = 10.0f;
}

void APEProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &APEProjectileBase::OnHit);
	
	// 프로젝타일 수명 타이머 설정
	GetWorldTimerManager().SetTimer(
		ProjectileLifetimeTimer,
		this,
		&APEProjectileBase::OnProjectileExpired,
		ProjectileLifetime,
		false
	);
}

void APEProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (GetWorldTimerManager().IsTimerActive(ProjectileLifetimeTimer))
	{
		GetWorldTimerManager().ClearTimer(ProjectileLifetimeTimer);
	}
}

void APEProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void APEProjectileBase::Launch(AActor* InInstigator, const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	ProjectileStats = AttackStats;
	InstigatorActor = InInstigator;
	
	SetActorLocation(StartLocation);
	
	// CollisionComponent의 Object Type을 AttackStats의 ProjectileCollision으로 설정
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionObjectType(AttackStats.ProjectileCollisionChannel);
	}
	
	if (ProjectileMovement)
	{
		float Speed = AttackStats.ProjectileSpeed > 0 ? AttackStats.ProjectileSpeed : 1000.0f;
		ProjectileMovement->InitialSpeed = Speed;
		ProjectileMovement->MaxSpeed = Speed;
		
		// 방향 설정 및 발사
		FVector LaunchDirection = Direction.GetSafeNormal();
		ProjectileMovement->Velocity = LaunchDirection * Speed;
		
		UE_LOG(LogTemp, Log, TEXT("Projectile launched with speed: %f, direction: %s"), 
			Speed, *LaunchDirection.ToString());
	}
}

AActor* APEProjectileBase::GetInstigator() const
{
	return InstigatorActor;
}

void APEProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// 자기 자신이나 소유자와의 충돌은 무시
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile hit: %s at location: %s"), 
			*OtherActor->GetName(), *Hit.Location.ToString());

		// 충돌한 액터가 PEReceiveAttackComponent를 가지고 있는지 확인
		if (UPEReceiveAttackComponent* ReceiveAttackComponent = OtherActor->FindComponentByClass<UPEReceiveAttackComponent>())
		{
			// 데미지 전달
			ReceiveAttackComponent->ReceiveDamage(
				ProjectileStats.DamageAmount,
				Hit.Location,
				Hit.Normal,
				InstigatorActor
			);
			UE_LOG(LogTemp, Log, TEXT("Damage applied: %f to %s"), 
				ProjectileStats.DamageAmount, *OtherActor->GetName());

			
		}
	}

	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitEffect,
			Hit.Location
		);
	}

	// 사운드 이펙트
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HitSound,
			Hit.Location
		);
	}
	
	OnProjectileExpired();
}

void APEProjectileBase::OnProjectileExpired()
{
	if (GetWorldTimerManager().IsTimerActive(ProjectileLifetimeTimer))
	{
		GetWorldTimerManager().ClearTimer(ProjectileLifetimeTimer);
	}
	
	Destroy();
}
