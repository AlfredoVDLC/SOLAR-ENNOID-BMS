# Versiones Solar ENNOID BMS

## 5.2.1-rc1 — 2026-11-05

Estado: candidata para pruebas de banco. No validada en la batería real.
Rama: `feature/selectable-lem-current`. Protocolo de configuración 5.2 conservado;
el sufijo de firmware y el número de prueba identifican esta revisión.

### Cambios

- BMS Tool: Signals → Current data source permite seleccionar el shunt existente
  o `LEM CAB500 (CAN, fail-stop)` (valor 6). Se conservan los índices anteriores
  y el tamaño/orden de la configuración almacenada; el valor predeterminado sigue
  siendo el shunt. Se mantienen las opciones antiguas por compatibilidad.
- En modo LEM la corriente de control alimenta las protecciones, potencia,
  integración de SoC y telemetría Victron. La pantalla sigue mostrando el shunt
  físico y el LEM por separado.
- CAB500: ID estándar 0x3C2, DLC 8, trama de datos, corriente en los bytes 0–3
  big endian con offset 0x80000000 y escala mA, error en bit 0 del byte 4.
  Formato heredado de la implementación contrastada con la captura existente.
- Arranque/selección LEM: salidas bloqueadas hasta 1 s de medidas frescas
  continuas; si no se consigue en 3 s se enclava el fallo de sensor.
- Una medida con antigüedad >=200 ms, error declarado o trama mal formada se
  considera inválida. Después de habilitar el LEM, la siguiente detección de
  medida inválida enclava el fallo. La actuación depende además de la latencia
  del bucle principal; no es una protección instantánea frente a cortocircuitos.
- Fallo: desactiva precarga, descarga LV/HV, carga y bypass de carga donde exista.
  No hay respaldo automático al shunt ni anulación por forced-on. Se entra en
  el estado de error existente y su posterior apagado configurado. Solo reiniciar
  el BMS permite rearmar; cambiar de fuente con el fallo enclavado no lo borra.
- Se conserva la última corriente al perder la medida, pero se suspende el
  conteo de culombios mientras no es válida. El SoC posterior contiene un
  intervalo desconocido: no se reconstruye energía no medida.
- Mientras la fuente no está lista: límites Victron de carga/descarga a 0,
  no se transmite 0x356 con corriente obsoleta, se suspenden los estados
  VESC/DieBie y el mensaje 0x35A de alarmas vacías, y el cargador CANopen recibe
  corriente 0 y ready=false. Si todo el bus falla, la desconexión depende de las
  salidas locales, no de que Venus reciba esos mensajes.
- Temporizador Victron separado del protocolo DieBieEngineering. El watchdog
  CAN considera actividad cualquier trama recibida, también las estándar LEM;
  antes podía reinicializar el periférico cada segundo con un único ID estándar.
- En modo LEM se comprueba sobrecorriente en ambos sentidos mediante magnitud.

### Archivos y uso

- `releases/5.2.1-rc1/SolarEnnoid_BMS-5.2.1-rc1-preserve-config.hex` excluye la
  sección `.fill` de EEPROM emulada (0x08000800–0x080017FF).
- El `main.hex` generado por el makefile original SÍ incluye esa zona. Para
  conservar ajustes usar la variante preserve-config con programación por páginas
  y **sin borrado completo del chip**. Exportar antes la configuración en BMS Tool.
- BMS Tool debe recompilarse para que aparezca la opción nueva. Cambiar el XML
  de fuentes no cambia el EXE antiguo. GitHub Actions genera un paquete Windows
  con EXE y dependencias Qt, accesible desde el artefacto de la ejecución.
- Leer primero la configuración actual, cambiar solo Current data source,
  escribirla y reiniciar el BMS. Para volver al shunt seleccionar su opción,
  guardar y reiniciar. No cargar valores predeterminados como parte del cambio.
- No seleccionar LEM con firmware anterior: ese valor no estaba implementado.
- LEM, BMS y GX deben compartir la velocidad CAN efectiva. No se cambia la
  velocidad guardada ni se reconfigura el sensor desde este firmware.

### Validación realizada

- Compilación completa `make -B -j4` para el hardware predeterminado ENNOID_LV,
  STM32F303xC, con arm-none-eabi-gcc 13.2.1. Existen advertencias heredadas del
  proyecto; compilar no equivale a validar el conjunto de protecciones.
- Pruebas host de decodificación (0, +10 y -10 A, error), habilitación,
  interrupciones en arranque, timeout inicial, enclavamiento y rollover del reloj.
- Harness que ejecuta las funciones reales del firmware: selección LEM,
  bloqueo de todas las salidas, forced-on, reintentos, ausencia de fallback y
  funcionamiento del shunt tras reiniciar.
- XML de BMS Tool analizado, siete opciones con índices 0–6; recursos Qt presentes.

### Pruebas de banco pendientes antes de uso normal

1. Conservar configuración y firmware anterior; confirmar placa ENNOID_LV.
2. En shunt verificar que pantalla, carga/descarga y Venus mantienen el comportamiento.
3. Elegir LEM y reiniciar; verificar signo: carga positiva, descarga negativa,
   tanto en el dato de control como en CAN (la pantalla muestra magnitudes).
   Confirmar con pinza, cero y varios niveles de corriente en ambos sentidos.
4. Verificar 0x351/355/356/35E, intervalos y corriente recibida por Venus.
5. Arrancar sin LEM; comprobar salidas desactivadas y fallo al agotarse la espera.
6. Con banco de baja energía, interrumpir mensajes/alimentación LEM y verificar
   tiempos, salidas físicas, límites a cero cuando sea posible y error en Tool.
7. Reconectar LEM sin reiniciar: no debe reactivar. Reiniciar con LEM sano: debe
   volver a exigir la cualificación. Repetir con forced-on y tráfico CAN ajeno.
8. Validar corte de contactores con el hardware real y protección independiente
   de cortocircuito. No se ha programado ni probado hardware desde este chat.

No se ha añadido una codificación de fallo de sensor en 0x35A: el fallo se
reporta en pantalla y BMS Tool. Los límites cero y las salidas locales son las
actuaciones implementadas para ese fallo.

## Copia funcional previa — 2026-11-05

Commit `3c36b2f`, etiqueta `backup/2026-11-05-dual-current`.
Incluye el estado existente, cambios locales y fuentes de BMS Tool antes de
esta implementación. Conserva el firmware que el usuario confirmó operativo
con ambas lecturas en pantalla y recepción en Venus GX. Los directorios PCB,
la distribución binaria de Tool y capturas no añadidas siguen intactos en disco.
