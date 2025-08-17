#include "Characters/Enemies/AI/PEAIBossCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

APEAIBossCharacter::APEAIBossCharacter()
{
    MeleeAttackAmount = 20;
    RangedAttackAmount = 15;
    MeleeAttackRange = 400.0f;
}
void APEAIBossCharacter::BeginPlay()
{
	Super::BeginPlay();
    AttackAmount = RangedAttackAmount;
    AttackRange = AttackRange;
	RangedAttackRange = AttackRange; // �������Ʈ ������Ƽ �� ���� �� AICharacter�� AttackRange�� ���

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        // ���� ���� ��Ÿ�ֵ� ���ε�
        if (MeleeAttackMontage1)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnBossAttackMontageCompleted);
        }
        if (MeleeAttackMontage2)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnBossAttackMontageCompleted);
        }
        if (RangedAttackMontage)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnBossAttackMontageCompleted);
        }
        if (PhaseTransitionMontage)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnPhaseTransitionCompleted);
        }
        if (SpecialSkillMontage)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnSpecialSkillMontageCompleted);
        }
    }
}

float APEAIBossCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
    {
        return 0.0f;
    }

    // �θ� Ŭ������ TakeDamage ȣ��
    float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // ������ ��ȯ üũ (ü���� ���� ���Ϸ� �������� ��)
    if (!bHasEnteredPhaseTwo && EnemyHealth <= (EnemyMaxHealth * 0.5f))
    {
        CheckPhaseTransition();
    }

    return Damage;
}

void APEAIBossCharacter::Die()
{
    if (bIsDead) return;

    UE_LOG(LogTemp, Warning, TEXT("Boss is dying!"));

    if (SpecialSkillTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpecialSkillTimerHandle);
    }

    if (MissileSpawnTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(MissileSpawnTimerHandle);
    }

    // Boss ��� ��������Ʈ ��ε�ĳ��Ʈ
    OnBossDeath.Broadcast();

    // �θ� Ŭ������ Die �Լ� ȣ��
    Super::Die();
}

void APEAIBossCharacter::CheckPhaseTransition()
{
    if (!bHasEnteredPhaseTwo && EnemyHealth <= (EnemyMaxHealth * 0.5f))
    {
        EnterPhaseTwo();
    }
}

void APEAIBossCharacter::EnterPhaseTwo()
{
    bHasEnteredPhaseTwo = true;
    CurrentPhase = EBossPhase::PhaseTwo;
    bIsInPhaseTransition = true;

    UE_LOG(LogTemp, Warning, TEXT("Boss entering Phase Two!"));

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->DisableMovement(); // ������ ��Ȱ��ȭ
        Movement->StopMovementImmediately(); // ��� ����
    }

    // AI Controller �����ӵ� ����
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopMovement(); // AI �̵� ��� �ߴ�

        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
        {
            BTComp->PauseLogic(TEXT("Phase Transition")); // �Ͻ� ����
        }
    }

    // 2������ ���� ����
    MeleeAttackAmount += 10; // ���ݷ� ����
    RangedAttackAmount += 5;

    // �̵� �ӵ� ����
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
		WalkSpeed += 50;
		RunSpeed += 100;
        Movement->MaxWalkSpeed += 100;
        
    }

    // ������ ��ȯ �ִϸ��̼� ��� (���û���)
    if (PhaseTransitionMontage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->StopAllMontages(0.2f);
            AnimInstance->Montage_Play(PhaseTransitionMontage, 1.0f);

            if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &APEAIBossCharacter::OnPhaseTransitionCompleted))
            {
                AnimInstance->OnMontageEnded.AddDynamic(this, &APEAIBossCharacter::OnPhaseTransitionCompleted);
            }
        }
    }

    // ��������Ʈ ��ε�ĳ��Ʈ
    OnBossPhaseTwo.Broadcast();
}

bool APEAIBossCharacter::ShouldUseMeleeAttack(float DistanceToTarget) const
{
    return DistanceToTarget <= MeleeAttackRange;
}

bool APEAIBossCharacter::PerformBossAttack(float DistanceToTarget)
{
    if (!AttackComponent || bIsDead || bIsAttacking)
    {
        return false;
    }

    // �Ÿ��� ���� ���� Ÿ�� ���� �� �ִϸ��̼� ���
    if (ShouldUseMeleeAttack(DistanceToTarget))
    {
        PlayRandomMeleeAttackAnimation();

        // ���� ���� �������� ����
        AttackAmount = MeleeAttackAmount;
        AttackRange = MeleeAttackRange;
    }
    else
    {
        // ���Ÿ� ����
		PlayRangedAttackAnimation();

        // ���Ÿ� ���� �������� ����
        AttackAmount = RangedAttackAmount;
        AttackRange = RangedAttackRange;
    }

    return true;
}

void APEAIBossCharacter::PlayRandomMeleeAttackAnimation()
{
    // ��� ������ ���� ���� ��Ÿ�� Ȯ��
    TArray<UAnimMontage*> MeleeAttackMontageList;

    if (MeleeAttackMontage1)
    {
        MeleeAttackMontageList.Add(MeleeAttackMontage1);
    }
    if (MeleeAttackMontage2)
    {
        MeleeAttackMontageList.Add(MeleeAttackMontage2);
    }

    // ��� ������ ��Ÿ�ְ� ������ �⺻ AttackMontage ���
    if (MeleeAttackMontageList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No melee attack montages available, using default AttackMontage"));
        PlayAttackAnimation();
        return;
    }

    if (bIsDead || bIsAttacking) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        // �����ϰ� ��Ÿ�� ����
        int32 RandomIndex = FMath::RandRange(0, MeleeAttackMontageList.Num() - 1);
        UAnimMontage* SelectedMontage = MeleeAttackMontageList[RandomIndex];

        bIsAttacking = true;
        AnimInstance->Montage_Play(SelectedMontage);

        UE_LOG(LogTemp, Log, TEXT("%s playing random melee attack animation %d"), *GetName(), RandomIndex + 1);
    }
}

void APEAIBossCharacter::PlayRangedAttackAnimation()
{
    if (!RangedAttackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("No ranged attack montage available, using default AttackMontage"));
        PlayAttackAnimation();
        return;
    }

    if (bIsDead || bIsAttacking) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        bIsAttacking = true;
        AnimInstance->Montage_Play(RangedAttackMontage);
        UE_LOG(LogTemp, Log, TEXT("%s playing ranged attack animation"), *GetName());
    }
}

void APEAIBossCharacter::OnBossAttackMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    // Boss ���� ��Ÿ�ֵ� üũ
    if (Montage == MeleeAttackMontage1 || Montage == MeleeAttackMontage2 || Montage == RangedAttackMontage)
    {
        bIsAttacking = false;
        UE_LOG(LogTemp, Log, TEXT("%s boss attack animation completed"), *GetName());
    }
    else
    {
        // �⺻ AttackMontage�� �θ� �Լ��� ó��
        Super::OnAttackMontageCompleted(Montage, bInterrupted);
    }
}

void APEAIBossCharacter::OnPhaseTransitionCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == PhaseTransitionMontage)
    {
        bIsInPhaseTransition = false;
        UE_LOG(LogTemp, Warning, TEXT("Boss phase transition completed!"));

        if (UCharacterMovementComponent* Movement = GetCharacterMovement())
        {
            Movement->SetMovementMode(MOVE_Walking); // �ȱ� ���� ����
        }

        if (AAIController* AIController = Cast<AAIController>(GetController()))
        {
            if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
            {
                BTComp->ResumeLogic(TEXT("Phase Transition"));
            }
        }
        // 2������ Ư�� ��ų Ÿ�̸� ����
        StartSpecialSkillTimer();
    }
}

void APEAIBossCharacter::StartSpecialSkillTimer()
{
    // �̹� Ÿ�̸Ӱ� ���� ���̸� ����
    if (SpecialSkillTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpecialSkillTimerHandle);
    }

    // Ư�� ��ų Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(
        SpecialSkillTimerHandle,
        this,
        &APEAIBossCharacter::StartSpecialSkill,
        SpecialSkillInterval,
        true // �ݺ� ����
    );

    UE_LOG(LogTemp, Warning, TEXT("Special skill timer started with interval: %f seconds"), SpecialSkillInterval);
}

void APEAIBossCharacter::StartSpecialSkill()
{
    // ����߰ų� 1������� ��ų ��� �� ��
    if (bIsDead || CurrentPhase != EBossPhase::PhaseTwo || bIsUsingSpecialSkill)
    {
        return;
    }

    bIsUsingSpecialSkill = true;

    UE_LOG(LogTemp, Warning, TEXT("Boss starting special missile strike!"));

    // AI �Ͻ� ����
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopMovement();

        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
        {
            BTComp->PauseLogic(TEXT("Special Skill"));
        }
    }

    // Ư�� ��ų �ִϸ��̼� ��� (�ִٸ�)
    if (SpecialSkillMontage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->Montage_Play(SpecialSkillMontage);
        }
    }
    else
    {
        // �ִϸ��̼��� ������ �ٷ� �̻��� ��Ʈ����ũ ����
        ExecuteMissileStrike();
    }

    // ȭ�鿡 ��� �޽���
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
            TEXT("BOSS SPECIAL ATTACK - MISSILE STRIKE INCOMING!"));
    }
}

void APEAIBossCharacter::ExecuteMissileStrike()
{
    if (!MissileBlueprintClass)
    {
        UE_LOG(LogTemp, Error, TEXT("MissileBlueprintClass is not set!"));
        OnSpecialSkillCompleted();
        return;
    }

    // ���� �̻��� �ε��� �ʱ�ȭ
    CurrentMissileIndex = 0;

    // �̻��� ���� Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(
        MissileSpawnTimerHandle,
        this,
        &APEAIBossCharacter::SpawnSingleMissile,
        MissileSpawnDelay,
        true // �ݺ� ����
    );

    UE_LOG(LogTemp, Warning, TEXT("Starting missile barrage with %d missiles"), MissileCount);
}

void APEAIBossCharacter::SpawnSingleMissile()
{
    if (CurrentMissileIndex >= MissileCount)
    {
        // ��� �̻��� ���� �Ϸ�
        GetWorld()->GetTimerManager().ClearTimer(MissileSpawnTimerHandle);

        // ��ų �Ϸ� ó�� (2�� ��)
        GetWorld()->GetTimerManager().SetTimer(
            MissileSpawnTimerHandle, // Ÿ�̸� ��Ȱ��
            this,
            &APEAIBossCharacter::OnSpecialSkillCompleted,
            2.0f,
            false
        );
        return;
    }

    // Boss �ֺ� ���� ��ġ ���
    FVector BossLocation = GetActorLocation();
    float RandomAngle = FMath::RandRange(0.0f, 360.0f);
    float RandomDistance = FMath::RandRange(200.0f, MissileStrikeRange);

    FVector TargetLocation = BossLocation + FVector(
        FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
        FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
        0.0f
    );

    // �̻��� ���� ��ġ (�ϴ� ��)
    FVector SpawnLocation = TargetLocation + FVector(0.0f, 0.0f, MissileSpawnHeight);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // �̻��� ����
    if (AActor* Missile = GetWorld()->SpawnActor<AActor>(MissileBlueprintClass, SpawnLocation, SpawnRotation))
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned missile %d at location: %s"),
            CurrentMissileIndex + 1, *SpawnLocation.ToString());
    }

    CurrentMissileIndex++;
}

void APEAIBossCharacter::OnSpecialSkillMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == SpecialSkillMontage)
    {
        // �ִϸ��̼� �Ϸ� �� �̻��� ��Ʈ����ũ ����
        ExecuteMissileStrike();
    }
}

void APEAIBossCharacter::OnSpecialSkillCompleted()
{
    bIsUsingSpecialSkill = false;

    // AI �簳
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
        {
            BTComp->ResumeLogic(TEXT("Special Skill"));
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Boss special skill completed, AI resumed"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            TEXT("Boss special attack completed!"));
    }
}