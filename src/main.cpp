#include <cstdint>
#include <cstring>

#include <mod/amlmod.h>
#include <mod/logger.h>

MYMOD(net.port.gangsterswatfix, Gangster SWAT Fix AML, 1.0, AMLPort)
NEEDGAME(com.rockstargames.gtasa)

// Verified in the supplied GTA SA 2.10 ARM64 libGTASA.so:
// CEntity::SetModelIndexNoCreate stores the model ID as a halfword at +0x32.
static constexpr std::uintptr_t kModelIndexOffset = 0x32;
static constexpr std::uint16_t kSwatModelId = 285;

// GTA SA weapon IDs: Micro SMG = 28, MP5 = 29.
static constexpr int kMicroSMG = 28;
static constexpr int kMP5 = 29;

static constexpr const char* kGiveWeaponSymbol =
    "_ZN4CPed10GiveWeaponE11eWeaponTypejb";

// GiveWeapon returns the weapon slot index (int) in this game library.
DECL_HOOK(int, GiveWeapon, void* ped, int weaponType,
          unsigned int ammo, bool addToGroup)
{
    if (ped != nullptr && weaponType == kMicroSMG)
    {
        std::uint16_t modelId = 0;
        const auto* pedBytes = static_cast<const std::uint8_t*>(ped);
        std::memcpy(&modelId, pedBytes + kModelIndexOffset, sizeof(modelId));

        if (modelId == kSwatModelId)
            weaponType = kMP5;
    }

    return GiveWeapon(ped, weaponType, ammo, addToGroup);
}

ON_MOD_PRELOAD()
{
    logger->SetTag("GangsterSWATFix");
}

ON_MOD_LOAD()
{
    const std::uintptr_t game = aml->GetLib("libGTASA.so");
    if (!game)
    {
        logger->Error("libGTASA.so not found; mod not installed");
        return;
    }

    const std::uintptr_t target = aml->GetSym(game, kGiveWeaponSymbol);
    if (!target)
    {
        logger->Error("CPed::GiveWeapon symbol not found; mod not installed");
        return;
    }

    if (!aml->Hook(reinterpret_cast<void*>(target),
                   reinterpret_cast<void*>(HookOf_GiveWeapon),
                   reinterpret_cast<void**>(&GiveWeapon)))
    {
        logger->Error("Could not hook CPed::GiveWeapon");
        return;
    }

    logger->Info("Loaded: SWAT Micro SMG is replaced with MP5");
}
