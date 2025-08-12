// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Projectile/PEProjectileBase.h"


APEProjectileBase::APEProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APEProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void APEProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


