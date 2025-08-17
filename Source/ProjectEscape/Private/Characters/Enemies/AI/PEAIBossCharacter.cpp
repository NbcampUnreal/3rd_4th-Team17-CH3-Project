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
	RangedAttackRange = AttackRange; // 블루프린트 프로퍼티 값 받을 시 AICharacter의 AttackRange를 사용

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        // 근접 공격 몽타주들 바인딩
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

    // 부모 클래스의 TakeDamage 호출
    float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 페이즈 전환 체크 (체력이 절반 이하로 떨어졌을 때)
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

    // Boss 사망 델리게이트 브로드캐스트
    OnBossDeath.Broadcast();

    // 부모 클래스의 Die 함수 호출
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
        Movement->DisableMovement(); // 움직임 비활성화
        Movement->StopMovementImmediately(); // 즉시 정지
    }

    // AI Controller 움직임도 정지
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopMovement(); // AI 이동 명령 중단

        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
        {
            BTComp->PauseLogic(TEXT("Phase Transition")); // 일시 정지
        }
    }

    // 2페이즈 스탯 조정
    MeleeAttackAmount += 10; // 공격력 증가
    RangedAttackAmount += 5;

    // 이동 속도 증가
    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
		WalkSpeed += 50;
		RunSpeed += 100;
        Movement->MaxWalkSpeed += 100;
        
    }

    // 페이즈 전환 애니메이션 재생 (선택사항)
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

    // 델리게이트 브로드캐스트
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

    // 거리에 따른 공격 타입 결정 및 애니메이션 재생
    if (ShouldUseMeleeAttack(DistanceToTarget))
    {
        PlayRandomMeleeAttackAnimation();

        // 근접 공격 데미지로 설정
        AttackAmount = MeleeAttackAmount;
        AttackRange = MeleeAttackRange;
    }
    else
    {
        // 원거리 공격
		PlayRangedAttackAnimation();

        // 원거리 공격 데미지로 설정
        AttackAmount = RangedAttackAmount;
        AttackRange = RangedAttackRange;
    }

    return true;
}

void APEAIBossCharacter::PlayRandomMeleeAttackAnimation()
{
    // 사용 가능한 근접 공격 몽타주 확인
    TArray<UAnimMontage*> MeleeAttackMontageList;

    if (MeleeAttackMontage1)
    {
        MeleeAttackMontageList.Add(MeleeAttackMontage1);
    }
    if (MeleeAttackMontage2)
    {
        MeleeAttackMontageList.Add(MeleeAttackMontage2);
    }

    // 사용 가능한 몽타주가 없으면 기본 AttackMontage 사용
    if (MeleeAttackMontageList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No melee attack montages available, using default AttackMontage"));
        PlayAttackAnimation();
        return;
    }

    if (bIsDead || bIsAttacking) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        // 랜덤하게 몽타주 선택
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
    // Boss 전용 몽타주들 체크
    if (Montage == MeleeAttackMontage1 || Montage == MeleeAttackMontage2 || Montage == RangedAttackMontage)
    {
        bIsAttacking = false;
        UE_LOG(LogTemp, Log, TEXT("%s boss attack animation completed"), *GetName());
    }
    else
    {
        // 기본 AttackMontage는 부모 함수로 처리
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
            Movement->SetMovementMode(MOVE_Walking); // 걷기 모드로 복원
        }

        if (AAIController* AIController = Cast<AAIController>(GetController()))
        {
            if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
            {
                BTComp->ResumeLogic(TEXT("Phase Transition"));
            }
        }
        // 2페이즈 특수 스킬 타이머 시작
        StartSpecialSkillTimer();
    }
}

void APEAIBossCharacter::StartSpecialSkillTimer()
{
    // 이미 타이머가 실행 중이면 정리
    if (SpecialSkillTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpecialSkillTimerHandle);
    }

    // 특수 스킬 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        SpecialSkillTimerHandle,
        this,
        &APEAIBossCharacter::StartSpecialSkill,
        SpecialSkillInterval,
        true // 반복 실행
    );

    UE_LOG(LogTemp, Warning, TEXT("Special skill timer started with interval: %f seconds"), SpecialSkillInterval);
}

void APEAIBossCharacter::StartSpecialSkill()
{
    // 사망했거나 1페이즈면 스킬 사용 안 함
    if (bIsDead || CurrentPhase != EBossPhase::PhaseTwo || bIsUsingSpecialSkill)
    {
        return;
    }

    bIsUsingSpecialSkill = true;

    UE_LOG(LogTemp, Warning, TEXT("Boss starting special missile strike!"));

    // AI 일시 정지
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        AIController->StopMovement();

        if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
        {
            BTComp->PauseLogic(TEXT("Special Skill"));
        }
    }

    // 특수 스킬 애니메이션 재생 (있다면)
    if (SpecialSkillMontage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            AnimInstance->Montage_Play(SpecialSkillMontage);
        }
    }
    else
    {
        // 애니메이션이 없으면 바로 미사일 스트라이크 실행
        ExecuteMissileStrike();
    }

    // 화면에 경고 메시지
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

    // 현재 미사일 인덱스 초기화
    CurrentMissileIndex = 0;

    // 미사일 스폰 타이머 시작
    GetWorld()->GetTimerManager().SetTimer(
        MissileSpawnTimerHandle,
        this,
        &APEAIBossCharacter::SpawnSingleMissile,
        MissileSpawnDelay,
        true // 반복 실행
    );

    UE_LOG(LogTemp, Warning, TEXT("Starting missile barrage with %d missiles"), MissileCount);
}

void APEAIBossCharacter::SpawnSingleMissile()
{
    if (CurrentMissileIndex >= MissileCount)
    {
        // 모든 미사일 스폰 완료
        GetWorld()->GetTimerManager().ClearTimer(MissileSpawnTimerHandle);

        // 스킬 완료 처리 (2초 후)
        GetWorld()->GetTimerManager().SetTimer(
            MissileSpawnTimerHandle, // 타이머 재활용
            this,
            &APEAIBossCharacter::OnSpecialSkillCompleted,
            2.0f,
            false
        );
        return;
    }

    // Boss 주변 랜덤 위치 계산
    FVector BossLocation = GetActorLocation();
    float RandomAngle = FMath::RandRange(0.0f, 360.0f);
    float RandomDistance = FMath::RandRange(200.0f, MissileStrikeRange);

    FVector TargetLocation = BossLocation + FVector(
        FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
        FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance,
        0.0f
    );

    // 미사일 스폰 위치 (하늘 위)
    FVector SpawnLocation = TargetLocation + FVector(0.0f, 0.0f, MissileSpawnHeight);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // 미사일 스폰
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
        // 애니메이션 완료 후 미사일 스트라이크 실행
        ExecuteMissileStrike();
    }
}

void APEAIBossCharacter::OnSpecialSkillCompleted()
{
    bIsUsingSpecialSkill = false;

    // AI 재개
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