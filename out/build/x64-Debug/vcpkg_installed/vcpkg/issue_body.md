Package: gmp:x64-windows -> 6.2.1#21

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.37.32824.0
-    vcpkg-tool version: 2023-09-15-ac02a9f660977426b8ec6392919fbb1d51b10998
    vcpkg-scripts version: 78ba9711d 2023-09-22 (9 days ago)

**To Reproduce**

`vcpkg install `
**Failure logs**

```
-- Using cached gmp-6.2.1.tar.xz.
-- Extracting source D:/EpicGames/Projects/FHCrypto/vcpkg/downloads/gmp-6.2.1.tar.xz
-- Applying patch asmflags.patch
-- Applying patch cross-tools.patch
CMake Error at scripts/cmake/z_vcpkg_apply_patches.cmake:34 (message):
  Applying patch failed: Checking patch Makefile.am...

  error: while searching for:



  EXTRA_DIST += bootstram_p.c



  fac_table.h: gen-fac$(EXEEXT_FOR_BUILD)

  	./gen-fac $(GMP_LIMB_BITS) $(GMP_NAIL_BITS) >fac_table.h || (rm -f
  fac_table.h; exit 1)

  BUILT_SOURCES += fac_table.h



  gen-fac$(EXEEXT_FOR_BUILD): gen-fac$(U_FOR_BUILD).c bootstram_p.c



  error: patch failed: Makefile.am:344

  error: Makefile.am: patch does not apply

  Checking patch acinclude.m4...

Call Stack (most recent call first):
  scripts/cmake/vcpkg_extract_source_archive.cmake:153 (z_vcpkg_apply_patches)
  ports/gmp/portfile.cmake:17 (vcpkg_extract_source_archive)
  scripts/ports.cmake:147 (include)



```
**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
  "name": "fhcrypto",
  "version": "1.0.0",
  "dependencies": [
    "gtest",
    "gmp",
    "pkgconf"
  ]
}

```
</details>
