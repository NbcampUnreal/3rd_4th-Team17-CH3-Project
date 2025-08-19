#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportPad.generated.h"

class UBoxComponent;

/**
 * 한 레벨 내부 전용 양방향 텔레포트 패드 (캐릭터 코드 수정 불필요)
 * - Trigger에 Pawn이 겹치면 TargetPad의 ExitPoint로 이동
 * - 왕복 튕김 방지(ReenterCooldown)
 * - 속도/회전 보존 옵션
 */
UCLASS()
class PROJECTESCAPE_API ATeleportPad : public AActor
{
    GENERATED_BODY()

public:
    ATeleportPad();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep,
        const FHitResult& Sweep);

    /** 텔레포트 가능 여부(쿨다운) */
    bool CanTeleport(AActor* Actor) const;

    /** 텔레포트 타임스탬프 기록 */
    void MarkTeleported(AActor* Actor) const;

    /** 도착 트랜스폼 계산 */
    FTransform GetExitTransform() const;

    /** Chaos 친화 안전 텔레포트 */
    bool DoTeleport_Safe(AActor* Actor, const FTransform& Dest);

public:
    /** 시각용 메쉬(선택) */
    UPROPERTY(VisibleAnywhere, Category = "Teleport")
    UStaticMeshComponent* PadMesh;

    /** 오버랩 감지 */
    UPROPERTY(VisibleAnywhere, Category = "Teleport")
    UBoxComponent* Trigger;

    /** 도착 기준 위치/회전(없으면 액터 트랜스폼 사용) */
    UPROPERTY(VisibleAnywhere, Category = "Teleport")
    USceneComponent* ExitPoint;

    /** 연결 대상 패드 (양방향이면 서로 지정) */
    UPROPERTY(EditInstanceOnly, Category = "Teleport")
    TWeakObjectPtr<ATeleportPad> TargetPad;

    /** 재진입 쿨다운(초) — 바운스 방지 */
    UPROPERTY(EditAnywhere, Category = "Options", meta = (ClampMin = "0.0"))
    float ReenterCooldown = 0.35f;

    /** 도착 시 회전 일치 */
    UPROPERTY(EditAnywhere, Category = "Options")
    bool bMatchRotation = true;

    /** 속도 보존 여부(캐릭터 무브먼트) */
    UPROPERTY(EditAnywhere, Category = "Options")
    bool bPreserveVelocity = true;

    /** 속도 보존 시 배율 */
    UPROPERTY(EditAnywhere, Category = "Options", meta = (EditCondition = "bPreserveVelocity"))
    float VelocityScale = 1.0f;

    /** 착지 충돌 방지 오프셋(문턱 등) */
    UPROPERTY(EditAnywhere, Category = "Options")
    FVector ArrivalOffset = FVector::ZeroVector;

private:
    /** 액터별 최근 텔레포트 시각(왕복 방지용) */
    static TMap<TWeakObjectPtr<AActor>, double> LastTeleportStamp;
};
