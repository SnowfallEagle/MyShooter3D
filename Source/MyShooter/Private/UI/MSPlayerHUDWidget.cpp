// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerHUDWidget.h"
#include "Components/MSHealthComponent.h"
#include "Components/MSWeaponComponent.h"

float UMSPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool UMSPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponUIData(UIData);
}

bool UMSPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const
{
    const auto WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponAmmoData(CurrentAmmo, DefaultAmmo);
}

bool UMSPlayerHUDWidget::IsPlayerAlive() const
{
    auto HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    return HealthComponent && !HealthComponent->IsDead();
}

bool UMSPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UMSPlayerHUDWidget::Initialize()
{
    auto HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UMSPlayerHUDWidget::OnHealthChanged);
    }

    return Super::Initialize();
}

void UMSPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
    if (DeltaHealth < 0)
    {
        OnDamageTaken();
    }
}
