# Gangster SWAT Fix — native AML port

Target: GTA San Andreas Android 2.10, ARM64 (`libGTASA.so` supplied by user).

This project is a native AML plugin source project. It is not a CLEO script.

## Behavior

When `CPed::GiveWeapon` is called for a ped whose model ID is 285 (SWAT), a
requested Micro SMG (weapon ID 28) is changed to MP5 (weapon ID 29). Other peds
and other weapons are passed through unchanged.

## Checks performed against the supplied game library

- ELF class/machine: ELF64, AArch64.
- `CPed::GiveWeapon` is exported at ELF virtual address `0x59525c` with symbol
  `_ZN4CPed10GiveWeaponE11eWeaponTypejb`.
- `CEntity::SetModelIndexNoCreate` writes the model ID using
  `strh w1, [x19, #0x32]`, confirming the model ID field offset `0x32`.
- `CPed::GiveWeapon` disassembly confirms it takes the ped pointer in `x0`,
  weapon type in `w1`, ammo in `w2`, and returns an integer slot index in `w0`.

The hook resolves the exported function by symbol at runtime rather than using
a hard-coded ASLR-sensitive address.

## Build requirements

- Android NDK with `ndk-build`
- AML Plugin SDK headers, with `mod/amlmod.h` and `mod/logger.h` available in
  `./aml-psdk/mod/`

Get the SDK from https://github.com/AndroidModLoader/aml-psdk

Then run `ndk-build` from this folder. The expected output is:

`libs/arm64-v8a/libgangsterswatfix.so`

Install that `.so` in the GTA SA AML mods directory:

`Android/data/com.rockstargames.gtasa/mods/`

## Status / limitations

The target function symbol and model-ID field offset were verified by
inspecting the supplied game library. This source has **not** been compiled
or tested in-game here because the Android NDK and AML SDK headers are not
installed in this build environment. Do not treat the source ZIP as an
installable plugin binary.
