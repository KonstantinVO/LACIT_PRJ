

#ifndef _DRV8353_DEFS_H
#define _DRV8353_DEFS_H

/*****************************************************************************/
// Register Addresses
/*****************************************************************************/
//DRV8353 Register Addresses
#define DRV8353_FAULT_ST1_ADDR                 0x00
#define DRV8353_VGS_ST2_ADDR                   0x01
#define DRV8353_DRIVER_CONTROL_ADDR            0x02
#define DRV8353_GATE_DRIVER_HS_ADDR            0x03
#define DRV8353_GATE_DRIVER_LS_ADDR            0x04
#define DRV8353_OCP_CONTROL_ADDR               0x05
#define DRV8353_CSA_CONTROL_ADDR               0x06
#define DRV8353_DRIVER_CONFIG_ADDR             0x07


//=============================================================================

// DRV8305 Warning and Watchdog Register bit definitions:
struct  DRV8353_FAULT_ST1_REG_BITS
{                                   // bit      description
       Uint16 VDS_LC:1;             // 0    Indicates VDS overcurrent fault on the C low-side MOSFET
       Uint16 VDS_HC:1;             // 1    Indicates VDS overcurrent fault on the C high-side MOSFET
       Uint16 VDS_LB:1;             // 2    Indicates VDS overcurrent fault on the B low-side MOSFET
       Uint16 VDS_HB:1;             // 3    Indicates VDS overcurrent fault on the B high-side MOSFET
       Uint16 VDS_LA:1;             // 4    Indicates VDS overcurrent fault on the A low-side MOSFET
       Uint16 VDS_HA:1;             // 5    Indicates VDS overcurrent fault on the A high-side MOSFET
       Uint16 OTSD:1;               // 6    Indicates overtemperature shutdown
       Uint16 UVLO:1;               // 7    Indicates undervoltage lockout fault condition
       Uint16 GDF:1;                // 8    Indicates gate drive fault condition
       Uint16 VDS_OCP:1;            // 9    Indicates VDS monitor overcurrent fault condition
       Uint16 FAULT:1;              // 10   Logic OR of FAULT status registers. Mirrors nFAULT pin.
       Uint16 Reserved:5;           // 15-11
};

typedef union  {
   Uint16                               all;
   struct DRV8353_FAULT_ST1_REG_BITS    bit;
} DRV8353_FAULT_ST1_REG;
// =============================

struct DRV8353_VGS_ST2_REG_BITS
{                               // bit      description
       Uint16 VGS_LC:1;         // 0    Indicates gate drive fault on the C low-side MOSFET
       Uint16 VGS_HC:1;         // 1    Indicates gate drive fault on the C high-side MOSFET
       Uint16 VGS_LB:1;         // 2    Indicates gate drive fault on the B low-side MOSFET
       Uint16 VGS_HB:1;         // 3    Indicates gate drive fault on the B high-side MOSFET
       Uint16 VGS_LA:1;         // 4    Indicates gate drive fault on the A low-side MOSFET
       Uint16 VGS_HA:1;         // 5    Indicates gate drive fault on the A high-side MOSFET
       Uint16 GDUV:1;           // 6    Indicates VCP charge pump and/or VGLS undervoltage fault condition
       Uint16 OTW:1;            // 7    Indicates overcurrent on phase C sense amplifier (DRV8353xS)
       Uint16 SC_OC:1;          // 8    Indicates overcurrent on phase B sense amplifier (DRV8353xS)
       Uint16 SB_OC:1;          // 9    Indicates overcurrent on phase A sense amplifier (DRV8353xS)
       Uint16 SA_OC:1;          // 10
       Uint16 Reserved:5;       // 15-11
};

typedef union  {
   Uint16                           all;
   struct DRV8353_VGS_ST2_REG_BITS  bit;
} DRV8353_VGS_ST2_REG;
// =============================

struct DRV8353_DRIVER_CONTROL_REG_BITS
{                               // bit      description
       Uint16 CLR_FLT:1;        // 0    Write a 1 to this bit to clear latched fault bits.This bit automatically resets after being writen.
       Uint16 BRAKE:1;          // 1    Write a 1 to this bit to turn on all three low-side MOSFETs This bit is ORed with the INLC (BRAKE) input in 1x PWM mode.
       Uint16 COAST:1;          // 2    Write a 1 to this bit to put all MOSFETs in the Hi-Z state
       Uint16 PWM1_DIR:1;       // 3    In 1x PWM mode this bit is ORed with the INHC (DIR) input
       Uint16 PWM1_COM:1;       // 4    0b = 1x PWM mode uses synchronous rectification / 1b = 1x PWM mode uses asynchronous rectification
       Uint16 PWM_MODE:2;       // 5:6  00b = 6x PWM Mode / 01b = 3x PWM mode / 10b = 1x PWM mode / 11b = Independent PWM mode
       Uint16 OTW_REP:1;        // 7    0b = OTW is not reported on nFAULT or the FAULT bit / 1b = OTW is reported on nFAULT and the FAULT bit
       Uint16 DIS_GDF:1;        // 8    0b = Gate drive fault is enabled / 1b = Gate drive fault is disabled
       Uint16 DIS_GDUV:1;       // 9    0b =VCP and VGLS undervoltage lockout fault is enabled / 1b = VCP and VGLS undervoltage lockout fault is disabled
       Uint16 OCP_ACT:1;        // 10   0b = Associated half-bridge is shutdown in response to VDS_OCP and SEN_OCP / 1b = All three half-bridges are shutdown in response to VDS_OCP and SEN_OCP
       Uint16 Reserved:5;       // 15-11
};

typedef union  {
   Uint16                                   all;
   struct DRV8353_DRIVER_CONTROL_REG_BITS   bit;
} DRV8353_DRIVER_CONTROL_REG;

// =============================

struct DRV8353_GATE_DRIVER_HS_REG_BITS
{                                   // bit      description
       Uint16 IDRIVEN_HS:4;         // 3:0      datasheet  p.60
       Uint16 IDRIVEP_HS:4;         // 7:4
       Uint16 LOCK:3;               // 10:8
       Uint16 Reserved:5;           // 15-11
};

typedef union  {
   Uint16                                   all;
   struct DRV8353_GATE_DRIVER_HS_REG_BITS   bit;
} DRV8353_GATE_DRIVER_HS_REG;

enum {
    drv8353_idriveN_hs_100mA_a = 0,
    drv8353_idriveN_hs_100mA_b = 1,
    drv8353_idriveN_hs_200mA   = 2,
    drv8353_idriveN_hs_300mA   = 3,
    drv8353_idriveN_hs_600mA   = 4,
    drv8353_idriveN_hs_700mA   = 5,
    drv8353_idriveN_hs_800mA   = 6,
    drv8353_idriveN_hs_900mA   = 7,
    drv8353_idriveN_hs_1100mA  = 8,
    drv8353_idriveN_hs_1200mA  = 9,
    drv8353_idriveN_hs_1300mA  = 10,
    drv8353_idriveN_hs_1400mA  = 11,
    drv8353_idriveN_hs_1700mA  = 12,
    drv8353_idriveN_hs_1800mA  = 13,
    drv8353_idriveN_hs_1900mA  = 14,
    drv8353_idriveN_hs_2000mA  = 15
};

enum {
    drv8353_idriveP_hs_50mA_a = 0,
    drv8353_idriveP_hs_50mA_b = 1,
    drv8353_idriveP_hs_100mA  = 2,
    drv8353_idriveP_hs_150mA  = 3,
    drv8353_idriveP_hs_300mA  = 4,
    drv8353_idriveP_hs_350mA  = 5,
    drv8353_idriveP_hs_400mA  = 6,
    drv8353_idriveP_hs_450mA  = 7,
    drv8353_idriveP_hs_550mA  = 8,
    drv8353_idriveP_hs_600mA  = 9,
    drv8353_idriveP_hs_650mA  = 10,
    drv8353_idriveP_hs_700mA  = 11,
    drv8353_idriveP_hs_850mA  = 12,
    drv8353_idriveP_hs_900mA  = 13,
    drv8353_idriveP_hs_950mA  = 14,
    drv8353_idriveP_hs_1000mA = 15
};

enum {
    drv8353_lock   = 6,
    drv8353_unlock = 3
};

// =============================

struct DRV8353_GATE_DRIVER_LS_REG_BITS
{                               // bit      description
       Uint16 IDRIVEN_LS:4;     // 3:0   datasheet  p.61
       Uint16 IDRIVEP_LS:4;     // 7:4
       Uint16 TDRIVE:2;         // 9:8
       Uint16 CBC:1;            // 10
       Uint16 Reserved:5;       // 15-11
};

typedef union  {
   Uint16                                    all;
   struct DRV8353_GATE_DRIVER_LS_REG_BITS    bit;
} DRV8353_GATE_DRIVER_LS_REG;

enum {
    drv8353_idriveN_ls_100mA_a = 0,
    drv8353_idriveN_ls_100mA_b = 1,
    drv8353_idriveN_ls_200mA   = 2,
    drv8353_idriveN_ls_300mA   = 3,
    drv8353_idriveN_ls_600mA   = 4,
    drv8353_idriveN_ls_700mA   = 5,
    drv8353_idriveN_ls_800mA   = 6,
    drv8353_idriveN_ls_900mA   = 7,
    drv8353_idriveN_ls_1100mA  = 8,
    drv8353_idriveN_ls_1200mA  = 9,
    drv8353_idriveN_ls_1300mA  = 10,
    drv8353_idriveN_ls_1400mA  = 11,
    drv8353_idriveN_ls_1700mA  = 12,
    drv8353_idriveN_ls_1800mA  = 13,
    drv8353_idriveN_ls_1900mA  = 14,
    drv8353_idriveN_ls_2000mA  = 15
};

enum {
    drv8353_idriveP_ls_50mA_a = 0,
    drv8353_idriveP_ls_50mA_b = 1,
    drv8353_idriveP_ls_100mA  = 2,
    drv8353_idriveP_ls_150mA  = 3,
    drv8353_idriveP_ls_300mA  = 4,
    drv8353_idriveP_ls_350mA  = 5,
    drv8353_idriveP_ls_400mA  = 6,
    drv8353_idriveP_ls_450mA  = 7,
    drv8353_idriveP_ls_550mA  = 8,
    drv8353_idriveP_ls_600mA  = 9,
    drv8353_idriveP_ls_650mA  = 10,
    rv8353_idriveP_ls_700mA   = 11,
    rv8353_idriveP_ls_850mA   = 12,
    rv8353_idriveP_ls_900mA   = 13,
    rv8353_idriveP_ls_950mA   = 14,
    drv8353_idriveP_ls_1000mA = 15
};

enum {
    drv8353_tdrive_500nS  = 0,
    drv8353_tdrive_1000nS = 1,
    drv8353_tdrive_2000nS = 2,
    drv8353_tdrive_4000nS = 3
};

enum {
    drv8353_cbc_fault_off  = 0,
    drv8353_cbc_fault_on = 1
};
// =============================

struct DRV8353_OCP_CONTROL_REG_BITS
{                                   // bit      description
       Uint16 VDS_LVL:4;            // 3:0   datasheet  p.61
       Uint16 OCP_DEG:2;            // 5:4
       Uint16 OCP_MODE:2;           // 7:6
       Uint16 DEAD_TIME:2;          // 9:8
       Uint16 TRETRY:1;             // 10
       Uint16 Reserved:5;           // 15-11
};

typedef union  {
   Uint16                                 all;
   struct DRV8353_OCP_CONTROL_REG_BITS    bit;
} DRV8353_OCP_CONTROL_REG;

enum {
    drv8353_vds_lvl_0_06v = 0,
    drv8353_vds_lvl_0_07v = 1,
    drv8353_vds_lvl_0_08v = 2,
    drv8353_vds_lvl_0_09v = 3,
    drv8353_vds_lvl_0_1v  = 4,
    drv8353_vds_lvl_0_2v  = 5,
    drv8353_vds_lvl_0_3v  = 6,
    drv8353_vds_lvl_0_4v  = 7,
    drv8353_vds_lvl_0_5v  = 8,
    drv8353_vds_lvl_0_6v  = 9,
    drv8353_vds_lvl_0_7v  = 10,
    drv8353_vds_lvl_0_8v  = 11,
    drv8353_vds_lvl_0_9v  = 12,
    drv8353_vds_lvl_1_0v  = 13,
    drv8353_vds_lvl_1_5v  = 14,
    drv8353_vds_lvl_2_0v  = 15
};

enum {
    drv8353_ocp_deg_1us = 0,
    drv8353_ocp_deg_2us = 1,
    drv8353_ocp_deg_4us = 2,
    drv8353_ocp_deg_8us = 3
};

enum {
    drv8353_ocp_mode_1 = 0, //datasheet  p.62
    drv8353_ocp_mode_2 = 1,
    drv8353_ocp_mode_3 = 2,
    drv8353_ocp_mode_4 = 3
};

enum {
    drv8353_deadTime_50nS  = 0,
    drv8353_deadTime_100nS = 1,
    drv8353_deadTime_200nS = 2,
    drv8353_deadTime_400nS = 3
};

enum {
    drv8353_tretry_8ms  = 0,
    drv8353_tretry_50us = 1
};

// =============================

struct DRV8353_CSA_CONTROL_REG_BITS
{                               // bit      description
       Uint16 SEN_LVL:2;        // 1:0    //datasheet  p.63
       Uint16 CSA_CAL_C:1;      // 2
       Uint16 CSA_CAL_B:1;      // 3
       Uint16 CSA_CAL_A:1;      // 4
       Uint16 DIS_SEN:1;        // 5
       Uint16 CSA_GAIN:2;       // 7:6
       Uint16 LS_REF:1;         // 8
       Uint16 VREF_DIV:1;       // 9
       Uint16 CSA_FET:1;        // 10
       Uint16 Reserved:5;       // 15-11
};

typedef union  {
   Uint16                               all;
   struct DRV8353_CSA_CONTROL_REG_BITS  bit;
} DRV8353_CSA_CONTROL_REG;

enum {
    drv8353_sevs_lvl_0_25v = 0,
    drv8353_sevs_lvl_0_5v  = 1,
    drv8353_sevs_lvl_0_75v = 2,
    drv8353_sevs_lvl_1v    = 3
};

enum {
    drv8353_csa_cal_c_normal = 0,
    drv8353_csa_cal_c_offset = 1
};

enum {
    drv8353_csa_cal_b_normal = 0,
    drv8353_csa_cal_b_offset = 1
};

enum {
    drv8353_csa_cal_a_normal = 0,
    drv8353_csa_cal_a_offset = 1
};

enum {
    drv8353_dis_sen_enabled  = 0,
    drv8353_dis_sen_disabled = 1
};

enum {
    drv8353_csa_gain_5v  = 0,
    drv8353_csa_gain_10v = 1,
    drv8353_csa_gain_20v = 2,
    drv8353_csa_gain_40v = 3
};

enum {
    drv8353_ls_ref_SHx_SPx = 0,
    drv8353_ls_ref_SHx_SNx = 1
};

enum {
    drv8353_vref_div_div0 = 0,
    drv8353_vref_div_div2 = 1
};

enum {
    drv8353_csa_fet_SPx= 0,
    drv8353_csa_fet_SHx = 1
};
// =============================

struct DRV8353_DRIVER_CONFIG_REG_BITS
{                             // bit      description
       Uint16 CAL_MODE:1;      // 0    //datasheet  p.64
       Uint16 RESERVED:9;      // 10:1
       Uint16 Reserved:5;      // 15-11
};

typedef union  {
   Uint16                                 all;
   struct DRV8353_DRIVER_CONFIG_REG_BITS  bit;
} DRV8353_DRIVER_CONFIG_REG;

enum {
    drv8353_cal_mode_manual     = 0,
    drv8353_cal_mode_auto_calib = 1
};

//=============================================================================
typedef struct  {
    Uint16                      Rsvd0;
    DRV8353_FAULT_ST1_REG       status_fault_reg;
    DRV8353_VGS_ST2_REG         status2_vgs_reg;
    DRV8353_DRIVER_CONTROL_REG  driver_control_reg;
    DRV8353_GATE_DRIVER_HS_REG  gate_driver_hs_reg;
    DRV8353_GATE_DRIVER_LS_REG  gate_driver_ls_reg;
    DRV8353_OCP_CONTROL_REG     ocp_control_reg;
    DRV8353_CSA_CONTROL_REG     csa_control_reg;
    DRV8353_DRIVER_CONFIG_REG   r_driver_config_reg;


    // DRV8353 variables
    Uint16 readCntrl1,
           readCntrl2,
           readCntrl3,
           readCntrl4,
           readCntrl5,
           readCntrl6,

           DRV_fault;

} DRV8353_Vars;

#define  DRV8353_DEFAULTS  {               \
        0,     /*  FAULT_ST1_REG      */   \
        0,     /*  VGS_ST2_REG        */   \
        0,     /*  DRIVER_CONTROL_REG */   \
        0,     /*  GATE_DRIVER_HS_REG */   \
        0,     /*  GATE_DRIVER_LS_REG */   \
        0,     /*  OCP_CONTROL_REG    */   \
        0,     /*  CSA_CONTROL_REG    */   \
        0,     /*  DRIVER_CONFIG_REG  */   \
                                           \
        0,     /*  readCntrl1         */   \
        0,     /*  readCntrl2         */   \
        0,     /*  readCntrl3         */   \
        0,     /*  readCntrl4         */   \
        0,     /*  readCntrl5         */   \
        0,     /*  readCntrl6         */   \
        0,     /*  DRV_fault         */    \
}

#endif
