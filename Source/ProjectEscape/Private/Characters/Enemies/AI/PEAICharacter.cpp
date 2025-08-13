// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEAICharacter.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Combat/Components/PEAttackHitscanComponent.h>
#include <Combat/Components/PEReceiveAttackComponent.h>
#include "Player/PEPlayerController.h"
#include "Items/Weapons/PEWeaponBase.h"
#include "Core/PEGameModeBase.h"

APEAICharacter::APEAICharacter()
{
	// AIController class setup
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 

	EnemyHealth = EnemyMaxHealth;

	// variable initialization
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->MaxWalkSpeed = WalkSpeed;
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = RotateSpeed;
		Movement->AirControl = AirControl;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AICharacterMovementComponent is nullptr!"));
	}

	AttackComponent = CreateDefaultSubobject<UPEAttackHitscanComponent>(TEXT("AttackComponent"));
	ReceiveComponent = CreateDefaultSubobject<UPEReceiveAttackComponent>(TEXT("ReceiveComponent"));
}

void APEAICharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (AIControllerClassBP == nullptr)
	{
		AIControllerClass = APEAIController::StaticClass();
	}
	else
	{
		AIControllerClass = AIControllerClassBP;
	}
}

// Called when the game starts or when spawned
void APEAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AICharacter has been spawned"));
}

void APEAICharacter::SetMovementSpeed(float NewSpeed)
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->MaxWalkSpeed = NewSpeed; // Update the walk speed
		WalkSpeed = NewSpeed; // Update the WalkSpeed property
		UE_LOG(LogTemp, Warning, TEXT("AICharacter movement speed set to: %f"), WalkSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set movement speed: Movement component not found"));
	}
}

void APEAICharacter::BeginDestroy()
{
	// AI 사망 시 델리게이트 브로드캐스트
	OnPawnDeath.Broadcast();

	Super::BeginDestroy();
}

float APEAICharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Take Damage"));

	bool bIsDead = false;
	if (ReceiveComponent)
	{
		if (Damage > 0.0f)
		{
			EnemyHealth -= Damage;
			if(EnemyHealth <= 0.0f)
			{
				EnemyHealth = 0.0f;
				UE_LOG(LogTemp, Display, TEXT("AICharacter is dead!"));
				OnPawnDeath.Broadcast(); // AI 사망 시 델리게이트 브로드캐스트
				this->Destroy(); // AI 캐릭터 제거
				bIsDead = true;
			}
		}
	}
	
	if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(DamageCauser))
	{
		if (APawn* WeaponOwner = Cast<APawn>(WeaponBase->GetItemOwner()))
		{
			if (APEPlayerController* PEPlayerController = Cast<APEPlayerController>(WeaponOwner->GetController()))
			{
				if (APEGameModeBase* PEGameModeBase = Cast<APEGameModeBase>(PEPlayerController->GetWorld()->GetAuthGameMode()))
				{
					if (bIsDead)
					{
						int32 KillScore = 500; // TODO: 
						PEGameModeBase->OnKillEnemy(KillScore);
						PEGameModeBase->OnDamageDealt(Damage);
						PEPlayerController->PlayKillMarkerAnimOfHUDWidget();
					}
					else
					{
						PEGameModeBase->OnDamageDealt(Damage);
						PEPlayerController->PlayHitMarkerAnimOfHUDWIdget();
					}
				}
			}
		}
	}
	return Damage;
}

