// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEAICharacter.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Combat/Components/PEAttackHitscanComponent.h>
#include <Combat/Components/PEReceiveAttackComponent.h>
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"

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
	ReceiveComponent->SetHiddenInGame(false);
	ReceiveComponent->SetupAttachment(RootComponent);
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

	EnemyHealth = EnemyMaxHealth;

	//�ִϸ��̼� ��Ÿ�� �Ϸ� ��������Ʈ ���ε� �߰�
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (AttackMontage)
			{
				AnimInstance->OnMontageEnded.AddDynamic(this, &APEAICharacter::OnAttackMontageCompleted);
			}
		}

	UE_LOG(LogTemp, Warning, TEXT("AICharacter has been spawned"));
}

void APEAICharacter::SetMovementSpeed(float NewSpeed)
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->MaxWalkSpeed = NewSpeed; // Update the walk speed
		//WalkSpeed = NewSpeed; // Update the WalkSpeed property
		UE_LOG(LogTemp, Warning, TEXT("AICharacter movement speed set to: %f"), NewSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set movement speed: Movement component not found"));
	}
}

void APEAICharacter::BeginDestroy()
{
	// AI ��� �� ��������Ʈ ��ε�ĳ��Ʈ
	OnPawnDeath.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Destory"));
	Super::BeginDestroy();
}

float APEAICharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Take Damage"));

	if (ReceiveComponent)
	{
		if (Damage > 0.0f)
		{
			EnemyHealth -= Damage;
			if(EnemyHealth <= 0.0f)
			{
				EnemyHealth = 0.0f;
				UE_LOG(LogTemp, Display, TEXT("AICharacter is dead!"));
				OnPawnDeath.Broadcast(); // AI ��� �� ��������Ʈ ��ε�ĳ��Ʈ
				Die(); // ��� ó��
			}

		}
	}
	
	return Damage;
}
bool APEAICharacter::PerformAttack()
{
	if (!AttackComponent || bIsDead || bIsAttacking)
	{
		return false;
	}
	PlayAttackAnimation();
	UE_LOG(LogTemp, Log, TEXT("%s triggered attack animation"), *GetName());

	return true;
}


void APEAICharacter::Die()
{
	if (bIsDead) 
	{ 
		return; 
	}

	bIsDead = true;
	PlayDeathAnimation();
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	// �ݸ��� ��Ȱ��ȭ
	SetActorEnableCollision(false);

	// 3�� �� �ı�
	SetLifeSpan(3.0f);
}

void APEAICharacter::PlayAttackAnimation()
{
	if (!AttackMontage || bIsDead || bIsAttacking) return;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		bIsAttacking = true;
		AnimInstance->Montage_Play(AttackMontage);
		UE_LOG(LogTemp, Log, TEXT("%s playing attack animation"), *GetName());
	}
}

void APEAICharacter::PlayDeathAnimation()
{
	if (!DeathMontage) return;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		// ��� ���� �ִϸ��̼� �ߴ�
		AnimInstance->StopAllMontages(0.2f);

		// ��� �ִϸ��̼� ���
		AnimInstance->Montage_Play(DeathMontage, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("%s playing death animation"), *GetName());
	}
}

void APEAICharacter::OnAttackMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		bIsAttacking = false;
		UE_LOG(LogTemp, Log, TEXT("%s attack animation completed"), *GetName());
	}
}