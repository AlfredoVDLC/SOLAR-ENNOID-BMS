# Compilación

## Firmware y pruebas host (Linux)

```sh
cc -std=c99 -Wall -Wextra -Werror -I Modules/Inc tests/test_current_source.c -o /tmp/test_current_source
/tmp/test_current_source
python3 tests/test_current_integration.py
python3 tests/test_can_freshness.py
make -B -j4
arm-none-eabi-objcopy -O ihex --remove-section=.fill main.elf SolarEnnoid_BMS-preserve-config.hex
```

Se exige recompilación completa porque el makefile original no calcula
las dependencias de cabeceras. No ejecutar `make clean` sin revisar sus reglas:
elimina todos los TXT de la raíz, además de artefactos de compilación.

## BMS Tool Windows

Workflow `.github/workflows/bms-tool-windows.yml`: Windows 2022, Qt 5.15.2,
MinGW 8.1 de 64 bits, qmake, mingw32-make y windeployqt. Se ejecuta en la rama
feature cuando cambian los fuentes de Tool, o manualmente desde Actions.
El paquete resultante se descarga como artefacto; no sustituye el EXE anterior.
El código conserva DT_VERSION numérico por compatibilidad y añade el sufijo
Solar 5.2.1-rc1 al identificador de la ventana.

El firmware debe conservar protocolo 5.2: no se ha añadido ningún byte a los
paquetes de configuración ni a la estructura persistente de EEPROM.

2026-11-06: primera compilación Windows completó el enlace del EXE; el
empaquetado falló al localizar el plugin de plataforma. El workflow fija las
rutas de windeployqt y qmake a la misma instalación Qt para evitar mezclar herramientas.

El paquete Windows copia las DLL de Qt y MinGW y los directorios plugins/qml
desde la instalación usada al compilar. qt.conf define rutas relativas. La
compilación incluye una prueba de arranque de 10 s sin Qt en PATH; no sustituye
las pruebas de comunicación serie y funcionamiento del BMS.

Validado el 2026-11-06: compilación, empaquetado y prueba de arranque Windows
correctos en https://github.com/AlfredoVDLC/SOLAR-ENNOID-BMS/actions/runs/42189193370 .
Artefacto: BMS-Tool-Windows-LEM-5.2.1-rc1. El ejecutable conserva el nombre
ENNOID-BMS-ToolV1.4.exe del proyecto original; la ventana identifica Solar 5.2.1-rc1.

Nombre actualizado a petición del usuario: SOLAR-ENNOID-BMS-ToolV1.4.1.exe.
Se ha renombrado el ejecutable Windows ya verificado, sin alterar su contenido,
y el TARGET de qmake genera ese nombre en las próximas compilaciones.
La versión del firmware permanece en 5.2.1-rc1.
