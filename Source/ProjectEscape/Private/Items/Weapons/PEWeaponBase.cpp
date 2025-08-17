// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponBase.h"

#include "Items/PEEquipmentType.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Items/Components/PEUseableComponent.h"
#include "Items/Components/PEInteractableComponent.h"
#include "Characters/Hero/Interface/PEWeaponAttachable.h"
#include "Characters/Hero/PEHero.h"
#include "Player/PEPlayerController.h"

APEWeaponBase::APEWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 생성 및 설정
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// 상호작용 컴포넌트 생성 및 설정
	InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(WeaponMesh);
	InteractableComponent->SetHiddenInGame(false);

	// 장착 컴포넌트 생성 및 설정
	HoldableComponent = CreateDefaultSubobject<UPEUseableComponent>(TEXT("HoldableComponent"));

	// 퀵슬롯 아이템 컴포넌트 생성 및 설정
	QuickSlotItemComponent = CreateDefaultSubobject<UPEQuickSlotItemComponent>(TEXT("QuickSlotItemComponent"));

	bIsFiring = false;
	bIsReloading = false;
	LastAttackTime = 0.0f;
	CurrentAmmoCount = 0;

}

void APEWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// AttackComponent 생성
	if (!AttackComponent)
	{
		AttackComponent = CreateAttackComponent();
		if (AttackComponent)
		{
			AttackComponent->RegisterComponent();
			UE_LOG(LogPE, Log, TEXT("AttackComponent created and registered for %s"), *GetName());
		}
		else
		{
			UE_LOG(LogPE, Error, TEXT("Failed to create AttackComponent for %s"), *GetName());
		}
	}

	// 데이터 테이블에서 무기 데이터 로드
	if (WeaponDataTable && !WeaponRowName.IsNone())
	{
		if (FPEWeaponData* WeaponData = WeaponDataTable->FindRow<FPEWeaponData>(WeaponRowName, TEXT("")))
		{
			WeaponStats = *WeaponData;
		}
	}
}

void APEWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
}

bool APEWeaponBase::TryReload()
{
	if (bIsReloading)
	{
		UE_LOG(LogPE, Warning, TEXT("Weapon is already reloading."));
		return false;
	}

	if (CurrentAmmoCount >= WeaponStats.MaxAmmo)
	{
		UE_LOG(LogPE, Warning, TEXT("Weapon already has maximum ammo: %d"), CurrentAmmoCount);
		return false;
	}
	
	if (IPEAttackable* IPEAttackableInterface = Cast<IPEAttackable>(WeaponOwnerActor))
	{
		AmmoComponent = IPEAttackableInterface->GetStorableItemComponent(WeaponStats.AmmoType);
		UE_LOG(LogPE, Log, TEXT("AmmoComponent set for %s with AmmoType: %s"), 
			*GetNameSafe(WeaponOwnerActor), *WeaponStats.AmmoType.ToString());
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("WeaponOwnerActor does not implement IPEAttackable interface"));
		return false;
	}

	if (!AmmoComponent.IsValid())
	{
		UE_LOG(LogPE, Warning, TEXT("AmmoComponent is not valid for %s"), *GetName());
		return false;
	}
	
	if (AmmoComponent->GetItemCount() > 0)
	{
		bIsReloading = true;
	
		GetWorldTimerManager().SetTimer(
			ReloadTimerHandle,
			this,
			&APEWeaponBase::PerformReload,
			WeaponStats.ReloadTime,
			false);
		UE_LOG(LogPE, Log, TEXT("Reloading..."));

		if (APEHero* PEHero = Cast<APEHero>(WeaponOwnerActor))
		{
			PEHero->PlayReloadAnimation(WeaponStats.ReloadTime);
		}
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("No ammo available for reloading."));
		return false;
	}

	return true;
}

void APEWeaponBase::PerformReload()
{
	if (!GetUseableComponent()->IsHolding())
	{
		bIsReloading = false;
		return;
	}
	
	if (AmmoComponent.IsValid() && AmmoComponent->GetItemCount() > 0)
	{
		int AmmoToReload = FMath::Min(WeaponStats.MaxAmmo - CurrentAmmoCount, AmmoComponent->GetItemCount());
		CurrentAmmoCount += AmmoToReload;
		AmmoComponent->ReduceItemCount(AmmoToReload);
	}
	
	bIsReloading = false;
	UE_LOG(LogPE, Log, TEXT("Reload complete. Current ammo: %d"), CurrentAmmoCount);
	
	BroadcastWeaponStateChanged();
}

void APEWeaponBase::CancleReload()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	bIsReloading = false;
}

void APEWeaponBase::Interact(AActor* Interactor)
{
	UE_LOG(LogPE, Warning, TEXT("Interact called on %s by %s"), *GetName(), *Interactor->GetName());
	
	WeaponOwnerActor = Interactor;
	if (IPEAttackable* AttackableInterface = Cast<IPEAttackable>(Interactor))
	{
		AttackComponent->SetAttackStartPoint(AttackableInterface->GetAttackStartPoint());
		UE_LOG(LogPE, Log, TEXT("AttackableInterface found and AttackStartPoint set for %s"), *GetNameSafe(Interactor));
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("Interactor does not implement IPEAttackable interface"));
	}
}

void APEWeaponBase::DoPrimaryAction(AActor* Holder)
{
	UE_LOG(LogTemp, Warning, TEXT("Use called on %s by %s"), *GetName(), *Holder->GetName());
	if (CurrentAmmoCount <= 0)
	{
		return; 
	}

	// NOTE: 무기 발사 불가능 상태가 많아지면 Tag로 전환하는 것을 고려해야 함
	if (bIsReloading)
	{
		return;
	}

	if (!WeaponStats.IsAutomatic && bIsFiring)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon is not automatic and already firing. Ignoring primary action."));
		return;
	}
	
	// FireRate 체크 (RPM을 초당 발사 횟수로 변환)
	if (WeaponStats.FireRate > 0.0f)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float TimeBetweenShots = 60.0f / WeaponStats.FireRate;
		
		if (CurrentTime - LastAttackTime < TimeBetweenShots)
		{
			// 아직 발사할 수 없음
			UE_LOG(LogTemp, Log, TEXT("Attack blocked by FireRate. Time since last attack: %f, Required interval: %f"), 
				CurrentTime - LastAttackTime, TimeBetweenShots);
			return;
		}
		
		LastAttackTime = CurrentTime;
	}

	FPEAttackStats AttackStats;
	AttackStats.AttackRange = WeaponStats.Range;
	AttackStats.DamageAmount = WeaponStats.Damage;
	AttackStats.SpreadAngle = WeaponStats.Spread;
	AttackStats.ProjectileClass = WeaponStats.ProjectileClass;
	AttackStats.ProjectileSpeed = WeaponStats.ProjectileSpeed;
	AttackStats.ExplosionRadius = WeaponStats.ExplosionRadius;
	AttackStats.ExplosionDelay = WeaponStats.ExplosionDelayTime;
	AttackStats.CollisionChannel = ECC_Visibility; //TODO: Collision 수정 필요

	// 1회 발사 시 몇 개의 탄환을 발사할지 설정 (e.g. 산탄총은 12개의 펠릿이 발사됌)
	for (int32 i = 0; i < WeaponStats.BulletsPerShot; ++i)
	{
		AttackComponent->ExcuteAttack(AttackStats);
	}

	if (APEHero* PEHero = Cast<APEHero>(WeaponOwnerActor))
	{
		if (WeaponStats.IsAutomatic)
		{
			float ShotIntervalSeconds = 60.0f / WeaponStats.FireRate;
			PEHero->PlayFireWeaponAnimation(ShotIntervalSeconds);
		}
		else
		{
			PEHero->PlayFireWeaponAnimation(0);
		}

		if (APEPlayerController* PEPlayerController = PEHero->GetController<APEPlayerController>())
		{
			PEPlayerController->PlayAimAnimOfHUDWidget();
		}
	}

	CurrentAmmoCount--;
	bIsFiring = true;
	
	BroadcastWeaponStateChanged();
}

void APEWeaponBase::CompletePrimaryAction(AActor* Holder)
{
	bIsFiring = false;
}

void APEWeaponBase::DoSecondaryAction(AActor* Holder)
{
	// NOTE: 보조 액션이 필요한 무기에만 구현됩니다.
}

void APEWeaponBase::DoTertiaryAction(AActor* Holder)
{	
	TryReload();
}

void APEWeaponBase::OnHand(AActor* NewOwner)
{
	AttachToOwner();

	// 2. 무기를 들었을 때 델리게이트 브로드캐스트
	BroadcastWeaponStateChanged();
}

void APEWeaponBase::OnRelease()
{
	bIsFiring = false;
	bIsReloading = false;

	DetachFromOwner();
	
	// 무기를 놓으면 맨손 상태가 되므로 빈 FPEEquipmentInfo를 브로드캐스트
	BroadcastEmptyWeaponState();
}

bool APEWeaponBase::IsInteractable() const
{
	if (WeaponOwnerActor) // 현재 주인이 있으면 상호작용 불가
	{
		return false;
	}
	return true;
}

UPEUseableComponent* APEWeaponBase::GetUseableComponent() const
{
	return HoldableComponent;
}

AActor* APEWeaponBase::GetItemOwner() const
{
	return WeaponOwnerActor;
}

void APEWeaponBase::OnDropped(const FVector& Location, const FRotator& Rotation)
{
	BroadcastEmptyWeaponState();
	
	WeaponOwnerActor = nullptr;
	OnRelease();
	
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	UE_LOG(LogTemp, Warning, TEXT("APEWeaponBase::OnDropped called on %s"), *GetName());
}

UPEQuickSlotItemComponent* APEWeaponBase::GetQuickSlotItemComponent() const
{
	return QuickSlotItemComponent;
}

EPEEquipmentType APEWeaponBase::GetEquipmentType() const
{
	return WeaponStats.Type;
}

UPEAttackBaseComponent* APEWeaponBase::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponBase::CreateAttackComponent() called"));
    return NewObject<UPEAttackBaseComponent>(this);
}

FPEEquipmentInfo APEWeaponBase::CreateCurrentEquipmentInfo() const
{
	FPEEquipmentInfo EquipmentInfo;
	EquipmentInfo.EquipmentName = WeaponStats.Name;
	EquipmentInfo.AmmoCount = FString::Printf(TEXT("%d/%d"), CurrentAmmoCount, WeaponStats.MaxAmmo);
	EquipmentInfo.EquipmentIcon = WeaponStats.IconTexture2D;
	EquipmentInfo.EquipmentDescription = FString::Printf(TEXT("Damage: %d, Range: %.1f"), 
		WeaponStats.Damage, WeaponStats.Range);

	return EquipmentInfo;
}

void APEWeaponBase::BroadcastWeaponStateChanged()
{
	FPEEquipmentInfo EquipmentInfo = CreateCurrentEquipmentInfo();

	if (APEHero* Hero = Cast<APEHero>(WeaponOwnerActor))
	{
		Hero->BroadCastEquipmentChanged(EquipmentInfo);
	
		// 델리게이트 브로드캐스트 정보 로그 출력 (테스트 용 코드)
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Name: %s, Count: %s, Description: %s"), 
			*EquipmentInfo.EquipmentName.ToString(),
			*EquipmentInfo.AmmoCount,
			*EquipmentInfo.EquipmentDescription);
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("BroadcastWeaponStateChanged: WeaponOwnerActor is not APEHero"));
	}
}

void APEWeaponBase::BroadcastEmptyWeaponState()
{
	FPEEquipmentInfo EmptyEquipmentInfo;
	EmptyEquipmentInfo.EquipmentName = FName(" ");
	EmptyEquipmentInfo.AmmoCount = TEXT(" ");
	EmptyEquipmentInfo.EquipmentDescription = TEXT(" ");
	EmptyEquipmentInfo.EquipmentIcon = nullptr;
	
	if (APEHero* Hero = Cast<APEHero>(WeaponOwnerActor))
	{
		Hero->BroadCastEquipmentChanged(EmptyEquipmentInfo);
	
		// 델리게이트 브로드캐스트 정보 로그 출력 (테스트 용 코드)
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Name: %s, Count: %s, Description: %s"), 
			*EmptyEquipmentInfo.EquipmentName.ToString(),
			*EmptyEquipmentInfo.AmmoCount,
			*EmptyEquipmentInfo.EquipmentDescription);
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Empty Weapon State"));
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("BroadcastWeaponStateChanged: WeaponOwnerActor is not APEHero"));
	}
}

void APEWeaponBase::AttachToOwner()
{
	// Remove existing weapon Actor
	if (AttachedActor)
	{
		DetachFromOwner();
	}

	if (WeaponStats.ActorToAttach)
	{
		if (IPEWeaponAttachable* AttachParent = Cast<IPEWeaponAttachable>(WeaponOwnerActor))
		{
			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters Params;
				AttachedActor = World->SpawnActor<AActor>(WeaponStats.ActorToAttach, Params);
				AttachParent->AttachWeapon(AttachedActor, WeaponStats.AttachTransform);
			}
		}
	}
}

void APEWeaponBase::DetachFromOwner()
{
	if (AttachedActor)
	{
		FDetachmentTransformRules Rule = FDetachmentTransformRules::KeepWorldTransform;
		AttachedActor->DetachFromActor(Rule);
		AttachedActor->Destroy();
		AttachedActor = nullptr;
	}
}
