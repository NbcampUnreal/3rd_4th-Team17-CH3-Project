// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/AI/PEAICharacter.h"
#include "Engine/TimerHandle.h"
#include "PEAIBossCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBossPhaseTwo);
DECLARE_MULTICAST_DELEGATE(FOnBossDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, float, float); // CurrentHealth, MaxHealth

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	PhaseOne UMETA(DisplayName = "Phase One"),
	PhaseTwo UMETA(DisplayName = "Phase Two")
};

UCLASS()
class PROJECTESCAPE_API APEAIBossCharacter : public APEAICharacter
{
	GENERATED_BODY()
	
public:
    APEAIBossCharacter();

    // ��������Ʈ
    FOnBossPhaseTwo OnBossPhaseTwo;
    FOnBossDeath OnBossDeath;
    FOnBossHealthChanged OnBossHealthChanged;

    // Boss ���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Combat")
    int32 MeleeAttackAmount = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Combat")
    int32 RangedAttackAmount = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Combat")
    float MeleeAttackRange = 400.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Boss|Combat")
    float RangedAttackRange = 1500.0f;

    // Ư�� ��ų ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    float SpecialSkillInterval = 30.0f; // Ư�� ��ų ��Ÿ�� (BP���� ���� ����)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    float MissileStrikeRange = 2000.0f; // �̻��� ���� ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    int32 MissileCount = 12; // �̻��� ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    float MissileSpawnDelay = 0.5f; // �̻��� �� ���� ������

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    TSubclassOf<AActor> MissileBlueprintClass; // BP �̻��� Ŭ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Special Skill")
    float MissileSpawnHeight = 2000.0f; // �̻��� ���� ����

    // ������ ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Boss|Phase")
    EBossPhase CurrentPhase = EBossPhase::PhaseOne;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Boss|Phase")
    bool bHasEnteredPhaseTwo = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Boss|Phase")
    bool bIsInPhaseTransition = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Special Skill")
    bool bIsUsingSpecialSkill = false;

    // �߰� �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Animation")
    TObjectPtr<UAnimMontage> MeleeAttackMontage1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Animation")
    TObjectPtr<UAnimMontage> MeleeAttackMontage2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Animation")
    TObjectPtr<UAnimMontage> RangedAttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Boss|Animation")
    TObjectPtr<UAnimMontage> PhaseTransitionMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Animation")
    TObjectPtr<UAnimMontage> SpecialSkillMontage; 

    // ���� Ÿ�� �Ǵ� �� ����
    UFUNCTION(BlueprintPure, Category = "Enemy|Boss|Combat")
    bool ShouldUseMeleeAttack(float DistanceToTarget) const;

    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Combat")
    bool PerformBossAttack(float DistanceToTarget);

    // Ư�� ��ų �Լ���
    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Special Skill")
    void StartSpecialSkill();

    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Special Skill")
    void ExecuteMissileStrike();

    UFUNCTION()
    void OnSpecialSkillMontageCompleted(UAnimMontage* Montage, bool bInterrupted);

	// ���� �ִϸ��̼� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Animation")
    void PlayRandomMeleeAttackAnimation();

    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Animation")
    void PlayRangedAttackAnimation();

    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Animation")
    void OnBossAttackMontageCompleted(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION(BlueprintCallable, Category = "Enemy|Boss|Animation")
    void OnPhaseTransitionCompleted(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION(BlueprintPure, Category = "Enemy|Boss|Phase")
    EBossPhase GetCurrentPhase() const { return CurrentPhase; }

    UFUNCTION(BlueprintPure, Category = "Enemy|Boss|Health")
    float GetCurrentHealth() const { return EnemyHealth; }

    UFUNCTION(BlueprintPure, Category = "Enemy|Boss|Health")
    float GetMaxHealth() const { return EnemyMaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Enemy|Boss|Health")
    float GetHealthPercent() const { return EnemyMaxHealth > 0 ? (EnemyHealth / EnemyMaxHealth) : 0.0f; }

protected:
	virtual void BeginPlay() override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void Die() override;

private:
    void CheckPhaseTransition();
    void EnterPhaseTwo();

    // Ư�� ��ų Ÿ�̸� ����
    FTimerHandle SpecialSkillTimerHandle;
    FTimerHandle MissileSpawnTimerHandle;
    int32 CurrentMissileIndex = 0;

    void StartSpecialSkillTimer();
    void SpawnSingleMissile();
    void OnSpecialSkillCompleted();
};
