#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Combat/Components/PEReceiveAttackComponent.h"
#include "PEAICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPawnDeath);

class APEAIController;
class UEnemyDropComponent;

UCLASS()
class PROJECTESCAPE_API APEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APEAICharacter();
	virtual void PreInitializeComponents() override;

	FOnPawnDeath OnPawnDeath;
	// Combat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UPEAttackBaseComponent> AttackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UPEReceiveAttackComponent> ReceiveComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Combat")
	USceneComponent* AttackStart;

	// Drop
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UEnemyDropComponent> DropComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	int32 AttackAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float AttackRange = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float EnemyMaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float EnemyHealth;

	// Status
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	TSubclassOf<APEAIController> AIControllerClassBP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float RunSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	FRotator RotateSpeed = FRotator(0.0f, 540.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float AirControl = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float AttackCoolTime = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Status")
	bool bIsDead = false;

	//시야 Sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sight")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sight")
	float LoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sight")
	float SightAngle = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sight")
	float SightDuration = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Sight")
	float InvestigateDuration = 5.0f; // 플레이어를 놓쳤을 때 조사하는 시간

	// 애니메이션 상태 (Runtime 정보)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Animation")
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Animation")
	float MovementSpeed = 0.0f;

	// 이펙트 및 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Effect")
	UParticleSystem* AttackParticleEffect;

	// 사운드를 블루프린트에서 설정 가능하도록  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Effect")
	USoundBase* AttackSoundEffect;

	void SetMovementSpeed(float NewSpeed);

	UFUNCTION()
	virtual void Die();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 애니메이션 몽타주 (BP에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;
public:	
	UFUNCTION(BlueprintCallable, Category = "Enemy|Animation")
	void OnAttackMontageCompleted(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Enemy|Animation")
	virtual bool PerformAttack();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Animation")
	void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable, Category = "Enemy|Animation")
	void PlayDeathAnimation();

	UFUNCTION(BlueprintPure, Category = "Enemy|Animation")
	bool GetIsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintPure, Category = "Enemy|Animation")
	bool GetIsDead() const { return bIsDead; }

};
