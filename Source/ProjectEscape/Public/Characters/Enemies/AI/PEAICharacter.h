#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Combat/Components/PEReceiveAttackComponent.h"
#include "PEAICharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPawnDeath);


class APEAIController;

UCLASS()
class PROJECTESCAPE_API APEAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APEAICharacter();
	virtual void PreInitializeComponents() override;

	FOnPawnDeath OnPawnDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UPEAttackBaseComponent> AttackComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UPEReceiveAttackComponent> ReceiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy||Status")
	int32 AttackAmount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float EnemyMaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	float EnemyHealth;


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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeadAnimMontage;


	void SetMovementSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	


};
