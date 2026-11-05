"""Exercise the actual firmware CAN accessor, including timeout and IRQ restoration."""
from pathlib import Path
import subprocess
import tempfile
root = Path(__file__).resolve().parents[1]
src = (root/'Modules/Src/modCAN.c').read_text()
start = src.index('bool modCANGetLEMCAB500Current(float *current) {')
end = src.index('\n}', start) + 2
code = r'''
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#define MOD_CAN_LEM_CAB500_TIMEOUT_MS 200U
static uint32_t tick, irq;
static bool modCANLEMCAB500MessageValid;
static uint32_t modCANLEMCAB500LastMessageTick;
static int32_t modCANLEMCAB500CurrentMilliAmps;
static uint32_t __get_PRIMASK(void) { return irq; }
static void __disable_irq(void) { irq = 1; }
static void __set_PRIMASK(uint32_t value) { irq = value; }
static uint32_t HAL_GetTick(void) { return tick; }
'''+src[start:end]+r'''
int main(void) {
 float amps = 1;
 assert(!modCANGetLEMCAB500Current(&amps));
 modCANLEMCAB500MessageValid = true;
 assert(modCANGetLEMCAB500Current(&amps) && amps == 0);
 assert(irq == 0);
 tick = 199; assert(modCANGetLEMCAB500Current(&amps));
 tick = 200; assert(!modCANGetLEMCAB500Current(&amps));
 modCANLEMCAB500LastMessageTick = UINT32_MAX - 100;
 tick = 98; assert(modCANGetLEMCAB500Current(&amps));
 tick = 99; assert(!modCANGetLEMCAB500Current(&amps));
 irq = 1; assert(!modCANGetLEMCAB500Current(NULL)); assert(irq == 1);
 tick = 0; modCANLEMCAB500LastMessageTick = 0;
 modCANLEMCAB500CurrentMilliAmps = -12500;
 assert(modCANGetLEMCAB500Current(&amps) && amps == -12.5f);
 modCANLEMCAB500MessageValid = false;
 assert(!modCANGetLEMCAB500Current(&amps));
 puts("actual CAN accessor: all checks passed");
}
'''
with tempfile.TemporaryDirectory() as tmp:
 c=Path(tmp)/'test.c'; c.write_text(code); exe=Path(tmp)/'test'
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(c),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)
