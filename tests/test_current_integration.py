"""Host harness executes the actual firmware source gating and output functions."""
from pathlib import Path
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parents[1]
src = (ROOT / 'Modules/Src/modPowerElectronics.c').read_text()
def function(signature):
    start = src.index(signature)
    brace = src.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (src[end] == '{') - (src[end] == '}')
        end += 1
    return src[start:end]
code = r'''
#include <assert.h>
#include <stdio.h>
#include "modCurrentSource.h"
#define HAS_PFET_OUTPUT 1
#define sourcePackCurrentCANLEMCAB500 6
#define FAULT_CODE_CURRENT_SENSOR 26
#define SWITCH_PRECHARGE 0
#define SWITCH_DISCHARGEHV 1
#define SWITCH_DISCHARGE 2
#define SWITCH_CHARGE 3
#define SWITCH_CHARGE_BYPASS 4
#define SWITCH_SET 1
#define SWITCH_RESET 0
typedef int driverHWSwitchesStateTypedef;
static struct { int packCurrentDataSource; } cfg;
static struct { float packCurrent; int chargeAllowed,disChargeLCAllowed,faultState;
 int preChargeDesired,disChargeDesired,chargeDesired,chargePFETDesired; } pack;
static __typeof__(cfg) *modPowerElectronicsGeneralConfigHandle = &cfg;
static __typeof__(pack) *modPowerElectronicsPackStateHandle = &pack;
static modCurrentSourceState currentSource;
static bool modPowerElectronicsAllowForcedOnState;
static bool fresh;
static float reading;
static uint32_t tick;
static int outputs[5];
static uint32_t HAL_GetTick(void) { return tick; }
static bool modCANGetLEMCAB500Current(float *p) { *p = reading; return fresh; }
static void driverHWSwitchesSetSwitchState(int pin,int state) { outputs[pin] = state; }
void modPowerElectronicsUpdateSwitches(void);
'''
for signature in ['bool modPowerElectronicsCurrentReady(void)',
                  'bool modPowerElectronicsCurrentFault(void)',
                  'static void modPowerElectronicsUpdateCurrentSource(void)',
                  'void modPowerElectronicsUpdateSwitches(void)']:
    code += '\n' + function(signature) + '\n'
code += r'''
static void allow(void) {
 pack.chargeAllowed = pack.disChargeLCAllowed = 1;
 pack.preChargeDesired = pack.disChargeDesired = pack.chargeDesired = pack.chargePFETDesired = 1;
}
static void off(void) { for(int i=0;i<5;i++) assert(outputs[i] == 0); }
int main(void) {
 cfg.packCurrentDataSource = 6;
 allow(); modPowerElectronicsAllowForcedOnState = true;
 modPowerElectronicsUpdateCurrentSource();
 modPowerElectronicsUpdateSwitches(); off();
 assert(!modPowerElectronicsCurrentReady());
 tick = 20; reading = -12.5f; fresh = true;
 modPowerElectronicsUpdateCurrentSource();
 assert(pack.packCurrent == -12.5f);
 tick = 1020; modPowerElectronicsUpdateCurrentSource();
 assert(modPowerElectronicsCurrentReady());
 allow(); modPowerElectronicsUpdateSwitches();
 for(int i=0;i<5;i++) assert(outputs[i] == 1);
 tick = 1220; fresh = false;
 modPowerElectronicsUpdateCurrentSource(); off();
 assert(modPowerElectronicsCurrentFault());
 assert(pack.packCurrent == -12.5f); /* retained, never invented zero */
 assert(pack.chargeAllowed == 0 && pack.disChargeLCAllowed == 0);
 /* retry logic and force-on cannot re-energize a failed sensor system */
 allow(); modPowerElectronicsUpdateSwitches(); off();
 fresh = true; tick = 5000; modPowerElectronicsUpdateCurrentSource(); off();
 cfg.packCurrentDataSource = 1; modPowerElectronicsUpdateCurrentSource(); off();
 /* reboot restores shunt mode independently of LEM */
 currentSource = (modCurrentSourceState){0}; fresh = false;
 modPowerElectronicsUpdateCurrentSource();
 assert(modPowerElectronicsCurrentReady());
 allow(); modPowerElectronicsUpdateSwitches();
 for(int i=0;i<5;i++) assert(outputs[i] == 1);
 puts("actual firmware output gating: all checks passed");
}
'''
with tempfile.TemporaryDirectory() as tmp:
    c = Path(tmp)/'harness.c'
    c.write_text(code)
    exe = Path(tmp)/'harness'
    subprocess.run(['cc','-std=gnu99','-Wall','-Wextra','-Werror','-I',str(ROOT/'Modules/Inc'),str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe)], check=True)
