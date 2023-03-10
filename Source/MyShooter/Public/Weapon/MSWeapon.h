// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, AMSWeapon*);

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bInfinite;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrosshairIcon;
};

UCLASS()
class MYSHOOTER_API AMSWeapon : public AActor
{
    GENERATED_BODY()

public:
    FOnClipEmptySignature OnClipEmpty;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo = { 15, 10, false };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

private:
    FAmmoData CurrentAmmo;

public:
    AMSWeapon();

    virtual void StartFire() {}
    virtual void StopFire() {}

    virtual void OnEquipped() {}
    virtual void OnUnequipped() {}

    void ChangeClip();
    bool TryToAddAmmo(int32 Clips);

    FORCEINLINE bool CanReload() const { return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0; }
    FORCEINLINE bool IsAmmoEmpty() const { return !CurrentAmmo.bInfinite && CurrentAmmo.Clips <= 0 && IsClipEmpty(); }
    FORCEINLINE bool IsClipEmpty() const { return CurrentAmmo.Bullets <= 0; }
    FORCEINLINE bool IsAmmoFull() const { return CurrentAmmo.Bullets == DefaultAmmo.Bullets && CurrentAmmo.Clips == DefaultAmmo.Clips; };

    const FWeaponUIData& GetUIData() const { return UIData; }
    void GetAmmoData(FAmmoData& InCurrentAmmo, FAmmoData& InDefaultAmmo) const;

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot() {}
    bool MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    void DecreaseAmmo();

    UNiagaraComponent* SpawnMuzzleFX();

    AController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FORCEINLINE FTransform GetMuzzleTransform() const { return WeaponMesh->GetSocketTransform(MuzzleSocketName); }
};
