// MyShooter Game, All Rights Reserved.

#include "Components/MSWeaponFXComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UMSWeaponFXComponent::UMSWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult) const
{
    // Find impact data
    const FImpactData* ImpactData = &DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMaterial))
        {
            ImpactData = &ImpactDataMap[PhysMaterial];
        }
    }

    // Spawn niagara effect
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),                       //
        ImpactData->NiagaraEffect,        //
        HitResult.ImpactPoint,            //
        HitResult.ImpactNormal.Rotation() //
    );

    // Spawn decal
    UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
        GetWorld(),                       //
        ImpactData->DecalData.Material,   //
        ImpactData->DecalData.Size,       //
        HitResult.ImpactPoint,            //
        HitResult.ImpactNormal.Rotation() //
    );

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData->DecalData.LifeTime, ImpactData->DecalData.FadeOutTime);

        if (ACharacter* Character = Cast<ACharacter>(HitResult.Actor.Get()))
        {
            DecalComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HitResult.BoneName);
        }
    }
}
