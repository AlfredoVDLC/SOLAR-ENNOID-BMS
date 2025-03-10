/*
	Copyright 2016 Benjamin Vedder	benjamin@vedder.se
	Copyright 2017 - 2018 Danny Bokma	danny@diebie.nl
	Copyright 2019 - 2020 Kevin Dionne	kevin.dionne@ennoid.me

	This file is part of the DieBieMS/ENNOID-BMS firmware.

	The DieBieMS/ENNOID-BMS firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The DieBieMS/ENNOID-BMS firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include "dataHelper.h"

#define systime_t uint32_t													// was not here

typedef enum {
	BALANCE_MODE_DISABLED = 0,
	BALANCE_MODE_CHARGING_ONLY,
	BALANCE_MODE_DURING_AND_AFTER_CHARGING,
	BALANCE_MODE_ALWAYS
} BMS_BALANCE_MODE;

typedef enum {
	HW_TYPE_VESC = 0,
	HW_TYPE_VESC_BMS,
	HW_TYPE_CUSTOM_MODULE,
	HW_TYPE_ENNOID_BMS
} HW_TYPE;

typedef enum {
	I_MEASURE_MODE_BMS = 0,
	I_MEASURE_MODE_VESC
} I_MEASURE_MODE;

// Data types
typedef enum {
   MC_STATE_OFF = 0,
   MC_STATE_DETECTING,
   MC_STATE_RUNNING,
   MC_STATE_FULL_BRAKE,
} mc_state;

typedef enum {
	PWM_MODE_NONSYNCHRONOUS_HISW = 0, // This mode is not recommended
	PWM_MODE_SYNCHRONOUS, // The recommended and most tested mode
	PWM_MODE_BIPOLAR // Some glitches occasionally, can kill MOSFETs
} mc_pwm_mode;

typedef enum {
	COMM_MODE_INTEGRATE = 0,
	COMM_MODE_DELAY
} mc_comm_mode;

typedef enum {
	SENSOR_MODE_SENSORLESS = 0,
	SENSOR_MODE_SENSORED,
	SENSOR_MODE_HYBRID
} mc_sensor_mode;

typedef enum {
	FOC_SENSOR_MODE_SENSORLESS = 0,
	FOC_SENSOR_MODE_ENCODER,
	FOC_SENSOR_MODE_HALL
} mc_foc_sensor_mode;

typedef enum {
	MOTOR_TYPE_BLDC = 0,
	MOTOR_TYPE_DC,
	MOTOR_TYPE_FOC
} mc_motor_type;

typedef enum {
	FAULT_CODE_NONE = 0,
	FAULT_CODE_PACK_OVER_VOLTAGE,
	FAULT_CODE_PACK_UNDER_VOLTAGE,
	FAULT_CODE_LOAD_OVER_VOLTAGE,
	FAULT_CODE_LOAD_UNDER_VOLTAGE,
	FAULT_CODE_CHARGER_OVER_VOLTAGE,
	FAULT_CODE_CHARGER_UNDER_VOLTAGE,
	FAULT_CODE_CELL_HARD_OVER_VOLTAGE,
	FAULT_CODE_CELL_HARD_UNDER_VOLTAGE,
	FAULT_CODE_CELL_SOFT_OVER_VOLTAGE,
	FAULT_CODE_CELL_SOFT_UNDER_VOLTAGE,
	FAULT_CODE_MAX_UVP_OVP_ERRORS,
	FAULT_CODE_MAX_UVT_OVT_ERRORS,
	FAULT_CODE_OVER_CURRENT,
	FAULT_CODE_OVER_TEMP_BMS,
	FAULT_CODE_UNDER_TEMP_BMS,
	FAULT_CODE_DISCHARGE_OVER_TEMP_CELLS,
	FAULT_CODE_DISCHARGE_UNDER_TEMP_CELLS,
	FAULT_CODE_CHARGE_OVER_TEMP_CELLS,
	FAULT_CODE_CHARGE_UNDER_TEMP_CELLS,
	FAULT_CODE_PRECHARGE_TIMEOUT,
	FAULT_CODE_DISCHARGE_RETRY,
	FAULT_CODE_CHARGE_RETRY,
	FAULT_CODE_CAN_DELAYED_POWER_DOWN,
	FAULT_CODE_NOT_USED_TIMEOUT,
	FAULT_CODE_CHARGER_DISCONNECT
} bms_fault_state;

typedef enum {
	CONTROL_MODE_DUTY = 0,
	CONTROL_MODE_SPEED,
	CONTROL_MODE_CURRENT,
	CONTROL_MODE_CURRENT_BRAKE,
	CONTROL_MODE_POS,
	CONTROL_MODE_HANDBRAKE,
	CONTROL_MODE_OPENLOOP,
	CONTROL_MODE_NONE
} mc_control_mode;

typedef enum {
	DISP_POS_MODE_NONE = 0,
	DISP_POS_MODE_INDUCTANCE,
	DISP_POS_MODE_OBSERVER,
	DISP_POS_MODE_ENCODER,
	DISP_POS_MODE_PID_POS,
	DISP_POS_MODE_PID_POS_ERROR,
	DISP_POS_MODE_ENCODER_OBSERVER_ERROR
} disp_pos_mode;

typedef enum {
	SENSOR_PORT_MODE_HALL = 0,
	SENSOR_PORT_MODE_ABI,
	SENSOR_PORT_MODE_AS5047_SPI
} sensor_port_mode;

typedef struct {
	float cycle_int_limit;
	float cycle_int_limit_running;
	float cycle_int_limit_max;
	float comm_time_sum;
	float comm_time_sum_min_rpm;
	int32_t comms;
	uint32_t time_at_comm;
} mc_rpm_dep_struct;

typedef enum {
	DRV8301_OC_LIMIT = 0,
	DRV8301_OC_LATCH_SHUTDOWN,
	DRV8301_OC_REPORT_ONLY,
	DRV8301_OC_DISABLED
} drv8301_oc_mode;

typedef enum {
	DEBUG_SAMPLING_OFF = 0,
	DEBUG_SAMPLING_NOW,
	DEBUG_SAMPLING_START,
	DEBUG_SAMPLING_TRIGGER_START,
	DEBUG_SAMPLING_TRIGGER_FAULT,
	DEBUG_SAMPLING_TRIGGER_START_NOSEND,
	DEBUG_SAMPLING_TRIGGER_FAULT_NOSEND,
	DEBUG_SAMPLING_SEND_LAST_SAMPLES
} debug_sampling_mode;

typedef struct {
	// Switching and drive
	mc_pwm_mode pwm_mode;
	mc_comm_mode comm_mode;
	mc_motor_type motor_type;
	mc_sensor_mode sensor_mode;
	// Limits
	float l_current_max;
	float l_current_min;
	float l_in_current_max;
	float l_in_current_min;
	float l_abs_current_max;
	float l_min_erpm;
	float l_max_erpm;
	float l_erpm_start;
	float l_max_erpm_fbrake;
	float l_max_erpm_fbrake_cc;
	float l_min_vin;
	float l_max_vin;
	float l_battery_cut_start;
	float l_battery_cut_end;
	bool l_slow_abs_current;
	float l_temp_fet_start;
	float l_temp_fet_end;
	float l_temp_motor_start;
	float l_temp_motor_end;
	float l_min_duty;
	float l_max_duty;
	float l_watt_max;
	float l_watt_min;
	// Overridden limits (Computed during runtime)
	float lo_current_max;
	float lo_current_min;
	float lo_in_current_max;
	float lo_in_current_min;
	float lo_current_motor_max_now;
	float lo_current_motor_min_now;
	// Sensorless (bldc)
	float sl_min_erpm;
	float sl_min_erpm_cycle_int_limit;
	float sl_max_fullbreak_current_dir_change;
	float sl_cycle_int_limit;
	float sl_phase_advance_at_br;
	float sl_cycle_int_rpm_br;
	float sl_bemf_coupling_k;
	// Hall sensor
	int8_t hall_table[8];
	float hall_sl_erpm;
	// FOC
	float foc_current_kp;
	float foc_current_ki;
	float foc_f_sw;
	float foc_dt_us;
	float foc_encoder_offset;
	bool foc_encoder_inverted;
	float foc_encoder_ratio;
	float foc_motor_l;
	float foc_motor_r;
	float foc_motor_flux_linkage;
	float foc_observer_gain;
	float foc_observer_gain_slow;
	float foc_pll_kp;
	float foc_pll_ki;
	float foc_duty_dowmramp_kp;
	float foc_duty_dowmramp_ki;
	float foc_openloop_rpm;
	float foc_sl_openloop_hyst;
	float foc_sl_openloop_time;
	float foc_sl_d_current_duty;
	float foc_sl_d_current_factor;
	mc_foc_sensor_mode foc_sensor_mode;
	uint8_t foc_hall_table[8];
	float foc_sl_erpm;
	bool foc_sample_v0_v7;
	bool foc_sample_high_current;
	float foc_sat_comp;
	bool foc_temp_comp;
	float foc_temp_comp_base_temp;
	// Speed PID
	float s_pid_kp;
	float s_pid_ki;
	float s_pid_kd;
	float s_pid_min_erpm;
	bool s_pid_allow_braking;
	// Pos PID
	float p_pid_kp;
	float p_pid_ki;
	float p_pid_kd;
	float p_pid_ang_div;
	// Current controller
	float cc_startup_boost_duty;
	float cc_min_current;
	float cc_gain;
	float cc_ramp_step_max;
	// Misc
	int32_t m_fault_stop_time_ms;
	float m_duty_ramp_step;
	float m_current_backoff_gain;
	uint32_t m_encoder_counts;
	sensor_port_mode m_sensor_port_mode;
	bool m_invert_direction;
	drv8301_oc_mode m_drv8301_oc_mode;
	int m_drv8301_oc_adj;
	float m_bldc_f_sw_min;
	float m_bldc_f_sw_max;
	float m_dc_f_sw;
} mc_configuration;

// Applications to use
typedef enum {
	APP_NONE = 0,
	APP_PPM,
	APP_ADC,
	APP_UART,
	APP_PPM_UART,
	APP_ADC_UART,
	APP_NUNCHUK,
	APP_NRF,
	APP_CUSTOM
} app_use;

// Throttle curve mode
typedef enum {
	THR_EXP_EXPO = 0,
	THR_EXP_NATURAL,
	THR_EXP_POLY
} thr_exp_mode;

// PPM control types
typedef enum {
	PPM_CTRL_TYPE_NONE = 0,
	PPM_CTRL_TYPE_CURRENT,
	PPM_CTRL_TYPE_CURRENT_NOREV,
	PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE,
	PPM_CTRL_TYPE_DUTY,
	PPM_CTRL_TYPE_DUTY_NOREV,
	PPM_CTRL_TYPE_PID,
	PPM_CTRL_TYPE_PID_NOREV
} ppm_control_type;

typedef struct {
	ppm_control_type ctrl_type;
	float pid_max_erpm;
	float hyst;
	float pulse_start;
	float pulse_end;
	float pulse_center;
	bool median_filter;
	bool safe_start;
	float throttle_exp;
	thr_exp_mode throttle_exp_mode;
	float ramp_time_pos;
	float ramp_time_neg;
	bool multi_esc;
	bool tc;
	float tc_max_diff;
} ppm_config;

// ADC control types
typedef enum {
	ADC_CTRL_TYPE_NONE = 0,
	ADC_CTRL_TYPE_CURRENT,
	ADC_CTRL_TYPE_CURRENT_REV_CENTER,
	ADC_CTRL_TYPE_CURRENT_REV_BUTTON,
	ADC_CTRL_TYPE_CURRENT_REV_BUTTON_BRAKE_ADC,
	ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_CENTER,
	ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_BUTTON,
	ADC_CTRL_TYPE_CURRENT_NOREV_BRAKE_ADC,
	ADC_CTRL_TYPE_DUTY,
	ADC_CTRL_TYPE_DUTY_REV_CENTER,
	ADC_CTRL_TYPE_DUTY_REV_BUTTON
} adc_control_type;

typedef struct {
	adc_control_type ctrl_type;
	float hyst;
	float voltage_start;
	float voltage_end;
	float voltage_center;
	float voltage2_start;
	float voltage2_end;
	bool use_filter;
	bool safe_start;
	bool cc_button_inverted;
	bool rev_button_inverted;
	bool voltage_inverted;
	bool voltage2_inverted;
	float throttle_exp;
	thr_exp_mode throttle_exp_mode;
	float ramp_time_pos;
	float ramp_time_neg;
	bool multi_esc;
	bool tc;
	float tc_max_diff;
	uint32_t update_rate_hz;
} adc_config;

// Nunchuk control types
typedef enum {
	CHUK_CTRL_TYPE_NONE = 0,
	CHUK_CTRL_TYPE_CURRENT,
	CHUK_CTRL_TYPE_CURRENT_NOREV
} chuk_control_type;

typedef struct {
	chuk_control_type ctrl_type;
	float hyst;
	float ramp_time_pos;
	float ramp_time_neg;
	float stick_erpm_per_s_in_cc;
	float throttle_exp;
	thr_exp_mode throttle_exp_mode;
	bool multi_esc;
	bool tc;
	float tc_max_diff;
} chuk_config;

// NRF Datatypes
typedef enum {
	NRF_SPEED_250K = 0,
	NRF_SPEED_1M,
	NRF_SPEED_2M
} NRF_SPEED;

typedef enum {
	NRF_POWER_M18DBM = 0,
	NRF_POWER_M12DBM,
	NRF_POWER_M6DBM,
	NRF_POWER_0DBM
} NRF_POWER;

typedef enum {
	NRF_AW_3 = 0,
	NRF_AW_4,
	NRF_AW_5
} NRF_AW;

typedef enum {
	NRF_CRC_DISABLED = 0,
	NRF_CRC_1B,
	NRF_CRC_2B
} NRF_CRC;

typedef enum {
	NRF_RETR_DELAY_250US = 0,
	NRF_RETR_DELAY_500US,
	NRF_RETR_DELAY_750US,
	NRF_RETR_DELAY_1000US,
	NRF_RETR_DELAY_1250US,
	NRF_RETR_DELAY_1500US,
	NRF_RETR_DELAY_1750US,
	NRF_RETR_DELAY_2000US,
	NRF_RETR_DELAY_2250US,
	NRF_RETR_DELAY_2500US,
	NRF_RETR_DELAY_2750US,
	NRF_RETR_DELAY_3000US,
	NRF_RETR_DELAY_3250US,
	NRF_RETR_DELAY_3500US,
	NRF_RETR_DELAY_3750US,
	NRF_RETR_DELAY_4000US
} NRF_RETR_DELAY;

typedef struct {
	NRF_SPEED speed;
	NRF_POWER power;
	NRF_CRC crc_type;
	NRF_RETR_DELAY retry_delay;
	unsigned char retries;
	unsigned char channel;
	unsigned char address[3];
	bool send_crc_ack;
} nrf_config;

typedef struct {
	// Settings
	uint8_t controller_id;
	uint32_t timeout_msec;
	float timeout_brake_current;
	bool send_can_status;
	uint32_t send_can_status_rate_hz;

	// Application to use
	app_use app_to_use;

	// PPM application settings
	ppm_config app_ppm_conf;

	// ADC application settings
	adc_config app_adc_conf;

	// UART application settings
	uint32_t app_uart_baudrate;

	// Nunchuk application settings
	chuk_config app_chuk_conf;

	// NRF application settings
	nrf_config app_nrf_conf;
} app_configuration;

// Communication commands
typedef enum {
	COMM_FW_VERSION = 0,
	COMM_JUMP_TO_BOOTLOADER,
	COMM_ERASE_NEW_APP,
	COMM_WRITE_NEW_APP_DATA,
	COMM_GET_VALUES,
	COMM_SET_DUTY,
	COMM_SET_CURRENT,
	COMM_SET_CURRENT_BRAKE,
	COMM_SET_RPM,
	COMM_SET_POS,
	COMM_SET_HANDBRAKE,
	COMM_SET_DETECT,
	COMM_SET_SERVO_POS,
	COMM_SET_MCCONF,
	COMM_GET_MCCONF,
	COMM_GET_MCCONF_DEFAULT,
	COMM_SET_APPCONF,
	COMM_GET_APPCONF,
	COMM_GET_APPCONF_DEFAULT,
	COMM_SAMPLE_PRINT,
	COMM_TERMINAL_CMD,
	COMM_PRINT,
	COMM_ROTOR_POSITION,
	COMM_EXPERIMENT_SAMPLE,
	COMM_DETECT_MOTOR_PARAM,
	COMM_DETECT_MOTOR_R_L,
	COMM_DETECT_MOTOR_FLUX_LINKAGE,
	COMM_DETECT_ENCODER,
	COMM_DETECT_HALL_FOC,
	COMM_REBOOT,
	COMM_ALIVE,
	COMM_GET_DECODED_PPM,
	COMM_GET_DECODED_ADC,
	COMM_GET_DECODED_CHUK,
	COMM_FORWARD_CAN,
	COMM_SET_CHUCK_DATA,
	COMM_CUSTOM_APP_DATA,
	COMM_NRF_START_PAIRING,
	COMM_GPD_SET_FSW,
	COMM_GPD_BUFFER_NOTIFY,
	COMM_GPD_BUFFER_SIZE_LEFT,
	COMM_GPD_FILL_BUFFER,
	COMM_GPD_OUTPUT_SAMPLE,
	COMM_GPD_SET_MODE,
	COMM_GPD_FILL_BUFFER_INT8,
	COMM_GPD_FILL_BUFFER_INT16,
	COMM_GPD_SET_BUFFER_INT_SCALE,
	COMM_GET_VALUES_SETUP,
	COMM_SET_MCCONF_TEMP,
	COMM_SET_MCCONF_TEMP_SETUP,
	COMM_GET_VALUES_SELECTIVE,
	COMM_GET_VALUES_SETUP_SELECTIVE,
	COMM_EXT_NRF_PRESENT,
	COMM_EXT_NRF_ESB_SET_CH_ADDR,
	COMM_EXT_NRF_ESB_SEND_DATA,
	COMM_EXT_NRF_ESB_RX_DATA,
	COMM_EXT_NRF_SET_ENABLED,
	COMM_DETECT_MOTOR_FLUX_LINKAGE_OPENLOOP,
	COMM_DETECT_APPLY_ALL_FOC,
	COMM_JUMP_TO_BOOTLOADER_ALL_CAN,
	COMM_ERASE_NEW_APP_ALL_CAN,
	COMM_WRITE_NEW_APP_DATA_ALL_CAN,
	COMM_PING_CAN,
	COMM_APP_DISABLE_OUTPUT,
	COMM_TERMINAL_CMD_SYNC,
	COMM_GET_IMU_DATA,
	COMM_BM_CONNECT,
	COMM_BM_ERASE_FLASH_ALL,
	COMM_BM_WRITE_FLASH,
	COMM_BM_REBOOT,
	COMM_BM_DISCONNECT,
	COMM_BM_MAP_PINS_DEFAULT,
	COMM_BM_MAP_PINS_NRF5X,
	COMM_ERASE_BOOTLOADER,
	COMM_ERASE_BOOTLOADER_ALL_CAN,
	COMM_PLOT_INIT,
	COMM_PLOT_DATA,
	COMM_PLOT_ADD_GRAPH,
	COMM_PLOT_SET_GRAPH,
	COMM_GET_DECODED_BALANCE,
	COMM_BM_MEM_READ,
	COMM_WRITE_NEW_APP_DATA_LZO,
	COMM_WRITE_NEW_APP_DATA_ALL_CAN_LZO,
	COMM_BM_WRITE_FLASH_LZO,
	COMM_SET_CURRENT_REL,
	COMM_CAN_FWD_FRAME,
	COMM_SET_BATTERY_CUT,
	COMM_SET_BLE_NAME,
	COMM_SET_BLE_PIN,
	COMM_SET_CAN_MODE,
	COMM_GET_IMU_CALIBRATION,
	COMM_GET_MCCONF_TEMP,

	// Custom configuration for hardware
	COMM_GET_CUSTOM_CONFIG_XML,
	COMM_GET_CUSTOM_CONFIG,
	COMM_GET_CUSTOM_CONFIG_DEFAULT,
	COMM_SET_CUSTOM_CONFIG,

	// VESC BMS commands
	COMM_BMS_GET_VALUES,
	COMM_BMS_SET_CHARGE_ALLOWED,
	COMM_BMS_SET_BALANCE_OVERRIDE,
	COMM_BMS_RESET_COUNTERS,
	COMM_BMS_FORCE_BALANCE,
	COMM_BMS_ZERO_CURRENT_OFFSET,

	// FW updates commands for different HW types
	COMM_JUMP_TO_BOOTLOADER_HW,
	COMM_ERASE_NEW_APP_HW,
	COMM_WRITE_NEW_APP_DATA_HW,
	COMM_ERASE_BOOTLOADER_HW,
	COMM_JUMP_TO_BOOTLOADER_ALL_CAN_HW,
	COMM_ERASE_NEW_APP_ALL_CAN_HW,
	COMM_WRITE_NEW_APP_DATA_ALL_CAN_HW,
	COMM_ERASE_BOOTLOADER_ALL_CAN_HW,

	COMM_SET_ODOMETER,
	
	// ENNOID-BMS specific
	COMM_EBMS_STORE_CONF = 150,
  	COMM_EBMS_GET_CELLS,
	COMM_EBMS_GET_AUX,
	COMM_EBMS_GET_EXP_TEMP,
	COMM_EBMS_SET_MCCONF,
	COMM_EBMS_GET_MCCONF,
	COMM_EBMS_GET_MCCONF_DEFAULT,
	COMM_EBMS_GET_VALUES,
} COMM_PACKET_ID;

typedef enum {
	CANIDStyleVESC = 0,
	CANIDStyleFoiler
} CAN_ID_STYLE;

typedef enum {
	OP_STATE_INIT = 0,		// 0
	OP_STATE_CHARGING,		// 1
	OP_STATE_PRE_CHARGE,		// 2
	OP_STATE_LOAD_ENABLED,		// 3
	OP_STATE_BATTERY_DEAD,		// 4
	OP_STATE_POWER_DOWN,		// 5
	OP_STATE_EXTERNAL,		// 6
	OP_STATE_ERROR,			// 7
	OP_STATE_ERROR_PRECHARGE,	// 8
	OP_STATE_BALANCING,		// 9
	OP_STATE_CHARGED,		// 10
	OP_STATE_FORCEON,		// 11
} OperationalStateTypedef;

typedef enum {
	opInit = 0,
	opChargerReset,
	opChargerSet,
	opCharging
} ChargerStateTypedef;

// CAN commands
typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5,
	CAN_PACKET_POLL_TS5700N8501_STATUS,
	CAN_PACKET_CONF_BATTERY_CUT,
	CAN_PACKET_CONF_STORE_BATTERY_CUT,
	CAN_PACKET_SHUTDOWN,
	CAN_PACKET_IO_BOARD_ADC_1_TO_4,
	CAN_PACKET_IO_BOARD_ADC_5_TO_8,
	CAN_PACKET_IO_BOARD_ADC_9_TO_12,
	CAN_PACKET_IO_BOARD_DIGITAL_IN,
	CAN_PACKET_IO_BOARD_SET_OUTPUT_DIGITAL,
	CAN_PACKET_IO_BOARD_SET_OUTPUT_PWM,
	CAN_PACKET_BMS_V_TOT,
	CAN_PACKET_BMS_I,
	CAN_PACKET_BMS_AH_WH,
	CAN_PACKET_BMS_V_CELL,
	CAN_PACKET_BMS_BAL,
	CAN_PACKET_BMS_TEMPS,
	CAN_PACKET_BMS_HUM,
	CAN_PACKET_BMS_SOC_SOH_TEMP_STAT,
	CAN_PACKET_PSW_STAT,
	CAN_PACKET_PSW_SWITCH,
	CAN_PACKET_BMS_HW_DATA_1,
	CAN_PACKET_BMS_HW_DATA_2,
	CAN_PACKET_BMS_HW_DATA_3,
	CAN_PACKET_BMS_HW_DATA_4,
	CAN_PACKET_BMS_HW_DATA_5,
	CAN_PACKET_BMS_AH_WH_CHG_TOTAL,
	CAN_PACKET_BMS_AH_WH_DIS_TOTAL,

	//ENNOIS-BMS specific 
	CAN_PACKET_EBMS_STATUS_MAIN_IV = 70,
	CAN_PACKET_EBMS_STATUS_CELLVOLTAGE,
	CAN_PACKET_EBMS_STATUS_THROTTLE_CH_DISCH_BOOL,
	CAN_PACKET_EBMS_STATUS_TEMPERATURES,
	CAN_PACKET_EBMS_STATUS_AUX_IV_SAFETY_WATCHDOG,
	CAN_PACKET_EBMS_KEEP_ALIVE_SAFETY,
	CAN_PACKET_EBMS_STATUS_TEMP_INDIVIDUAL,	
} CAN_PACKET_ID;

typedef struct {
	int id;
	systime_t rx_time;
	float rpm;
	float current;
	float duty;
} can_status_msg;

typedef struct {
	int id;
	systime_t rx_time;
	float amp_hours;
	float amp_hours_charged;
} can_status_msg_2;

typedef struct {
	int id;
	systime_t rx_time;
	float watt_hours;
	float watt_hours_charged;
} can_status_msg_3;

typedef struct {
	int id;
	systime_t rx_time;
	float temp_fet;
	float temp_motor;
	float current_in;
	float pid_pos_now;
} can_status_msg_4;

typedef struct {
	int id;
	systime_t rx_time;
	float v_in;
	int32_t tacho_value;
} can_status_msg_5;

typedef enum {
	MOTE_PACKET_BATT_LEVEL = 0,
	MOTE_PACKET_BUTTONS,
	MOTE_PACKET_ALIVE,
	MOTE_PACKET_FILL_RX_BUFFER,
	MOTE_PACKET_FILL_RX_BUFFER_LONG,
	MOTE_PACKET_PROCESS_RX_BUFFER,
	MOTE_PACKET_PROCESS_SHORT_BUFFER,
	MOTE_PACKET_PAIRING_INFO
} MOTE_PACKET;

typedef enum {
	NRF_PAIR_STARTED = 0,
	NRF_PAIR_OK,
	NRF_PAIR_FAIL
} NRF_PAIR_RES;

typedef enum {
	CELL_MON_NONE = 0,
	CELL_MON_LTC6811_1,
	CELL_MON_LTC6812_1,
	CELL_MON_LTC6813_1,
	CELL_MON_LTC6810_1
} configCellMonitorICTypeEnum;

typedef enum {
  	opStateExternal = 0,
	opStateExtNormal
} configExtEnableStateTypeEnum;

typedef enum {
  	electricVehicle = 0,
	energyStorage
} configBMSApplication;

typedef enum {
  opStateChargingModeCharging = 0,
	opStateChargingModeNormal
} configChargerEnableStateTypeEnum;

typedef enum {
  canSpeedBaud125k = 0,
	canSpeedBaud250k,
	canSpeedBaud500k
} configCANSpeedTypeEnum;

typedef enum {
	sourcePackVoltageNone = 0,
	sourcePackVoltageISL28022,
	sourcePackVoltageSumOfIndividualCellVoltages,
	sourcePackVoltageCANDieBieShunt,
	sourcePackVoltageCANIsabellenhutte,
	sourcePackVoltageINA226
} configPackVoltageDataSourceEnum;

typedef enum {
	sourcePackCurrentNone = 0,
	sourcePackCurrentISL28022,
	sourcePackCurrentCANDieBieShunt,
	sourcePackCurrentCANIsaBellenHuette,
	sourcePackCurrentINA226,
	sourcePackCurrentCANVESC
} configPackCurrentDataSourceEnum;

typedef enum {
	socNone = 0,
	socCoulomb,
	socCoulombAndCellVoltage
} configStateOfChargeMethodEnum;

typedef enum {
	basic = 0,
	advanced
} displayStyle;

typedef enum {
  buzzerSourceOff = 0,
  buzzerSourceOn,
  buzzerSourceAll,
  buzzerSourceLC,
  buzzerSourceSOA	
} buzzerSignalSourceEnum;

typedef enum {
	canEmitProtocolNone = 0,
  	canEmitProtocolDieBieEngineering,
	canEmitProtocolMGElectronics,
	canEmitProtocolVESC
} canEmitProtocol;

typedef struct {
	float   cellVoltage;
	uint8_t cellNumber;
	bool    cellBleedActive;
} cellMonitorCellsTypeDef;

typedef struct {
	float   auxVoltage;
	uint8_t auxNumber;
} auxMonitorTypeDef;

typedef struct {
	float   expVoltage;
	uint8_t expNumber;
} expMonitorTypeDef;

typedef struct {
  	float    voltages[12];
	uint16_t balanceMask;
	float    temperatures[8];
	float    humidity;
} cellMonitorModuleTypeDef;

typedef enum {
	disabled = 0,
	enabled,
	forced // For specific hardware with negative main contactor as precharge circuit
} switchState;

typedef enum {
	none = 0,
	si7020,
	htc1080
} humidityICTypeEnum;





typedef struct __attribute__((packed)) {
	// ID if this BMS (e.g. on the CAN-bus)
	uint8_t controller_id;


	configCANSpeedTypeEnum can_baud_rate;


	// Number of cells in series
	int cell_num;

	// Number of temperature sensors per module
	int temp_num;
	float balance_start_voltage;
	float balance_difference_threshold;
	float soft_overvoltage;
	float soft_undervoltage;
	float hard_overvoltage;
	float hard_undervoltage;

	// Only allow charging when the cell temperature is below this value
	float t_charge_max;
	float t_discharge_max;

	// Maximum allowed charging current
	float not_used_current_threshold;
	// Reset sleep timeout to this value at events that prevent sleeping
	int not_used_timeout;


} main_config_t;

#endif /* DATATYPES_H_ */
