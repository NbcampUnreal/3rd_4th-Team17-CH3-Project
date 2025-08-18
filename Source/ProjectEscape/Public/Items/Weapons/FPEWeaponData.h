// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Items/PEEquipmentType.h"
#include "UObject/Object.h"
#include "FPEWeaponData.generated.h"

class UPaperSprite;
class APEProjectileBase;
enum class EPEEquipmentType : uint8;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPEWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name = "DefaultWeapon";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPEEquipmentType Type = EPEEquipmentType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AmmoType = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APEProjectileBase> ProjectileClass = nullptr; // 투사체 방식일 때만 사용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionRadius = 100.0f; // 폭발 반경, 투사체가 폭발하는 경우에만 사용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionDelayTime = 1.0f; // 폭발 지연 시간, 투사체가 폭발하는 경우에만 사용
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAutomatic = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHitscan = true;	// 명시적으로 사용, 실제로는 HitScan 클래스로 구현해야 함
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMagazineReload = true; // 명시적으로 사용, 실제로는 무기 클래스 안에 구현해야 함

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInfiniteAmmo = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BulletsPerShot = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime = 2.0f;;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spread = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description = " ";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToAttach = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform AttachTransform = FTransform::Identity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconTexture2D = nullptr;
};