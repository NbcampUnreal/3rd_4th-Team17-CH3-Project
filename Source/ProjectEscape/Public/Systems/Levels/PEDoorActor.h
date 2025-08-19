#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEDoorActor.generated.h"

UENUM(BlueprintType)
enum class ESlideDirection : uint8
{
    LocalX_Pos   UMETA(DisplayName = "Forward (+X)"),
    LocalX_Neg   UMETA(DisplayName = "Backward (-X)"),
    LocalY_Pos   UMETA(DisplayName = "Right (+Y)"),
    LocalY_Neg   UMETA(DisplayName = "Left (-Y)"),
    LocalZ_Pos   UMETA(DisplayName = "Up (+Z)"),
    LocalZ_Neg   UMETA(DisplayName = "Down (-Z)")
};

UCLASS()
class PROJECTESCAPE_API APEDoorActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APEDoorActor();

    UPROPERTY(VisibleAnywhere) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* DoorMesh;

    // ▶ 디테일 패널에서 조절
    UPROPERTY(EditAnywhere, Category = "Slide")
    ESlideDirection Direction = ESlideDirection::LocalY_Pos;

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (ClampMin = "1.0"))
    float Distance = 200.f;                          // 슬라이드 거리(cm)

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (ClampMin = "0.05"))
    float MoveTime = 0.6f;                           // 여닫이 시간

    UPROPERTY(EditAnywhere, Category = "Slide")
    bool bAutoClose = false;

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (EditCondition = "bAutoClose", ClampMin = "0.1"))
    float AutoCloseDelay = 2.0f;

    // 멀티용 상태 복제
    UPROPERTY(ReplicatedUsing = OnRep_OpenState, VisibleInstanceOnly, Category = "Net")
    bool bOpen = false;

    UFUNCTION(BlueprintCallable) void ToggleOpen();
    UFUNCTION(BlueprintCallable) void Open();
    UFUNCTION(BlueprintCallable) void Close();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_OpenState();

private:
    // 이동 보간 상태
    bool   bMoving = false;
    double MoveStartTime = 0.0;

    FVector BaseLoc;     // 초기(닫힘) 위치(상대)
    FVector StartLoc;    // 이동 시작 시점 위치
    FVector TargetLoc;   // 목표 위치(열림/닫힘)

    FTimerHandle AutoCloseTimer;

    FVector DirVectorLocal() const;   // 방향 벡터(로컬)
    void    BeginMove(bool bToOpen);
    void    ApplyAtAlpha(float Alpha);
    void    ClearAutoClose();
    void    ScheduleAutoClose();

};