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

    // �� ������ �гο��� ����
    UPROPERTY(EditAnywhere, Category = "Slide")
    ESlideDirection Direction = ESlideDirection::LocalY_Pos;

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (ClampMin = "1.0"))
    float Distance = 200.f;                          // �����̵� �Ÿ�(cm)

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (ClampMin = "0.05"))
    float MoveTime = 0.6f;                           // ������ �ð�

    UPROPERTY(EditAnywhere, Category = "Slide")
    bool bAutoClose = false;

    UPROPERTY(EditAnywhere, Category = "Slide", meta = (EditCondition = "bAutoClose", ClampMin = "0.1"))
    float AutoCloseDelay = 2.0f;

    // ��Ƽ�� ���� ����
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
    // �̵� ���� ����
    bool   bMoving = false;
    double MoveStartTime = 0.0;

    FVector BaseLoc;     // �ʱ�(����) ��ġ(���)
    FVector StartLoc;    // �̵� ���� ���� ��ġ
    FVector TargetLoc;   // ��ǥ ��ġ(����/����)

    FTimerHandle AutoCloseTimer;

    FVector DirVectorLocal() const;   // ���� ����(����)
    void    BeginMove(bool bToOpen);
    void    ApplyAtAlpha(float Alpha);
    void    ClearAutoClose();
    void    ScheduleAutoClose();

};