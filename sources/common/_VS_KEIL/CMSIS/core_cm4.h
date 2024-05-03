#pragma once

#ifdef GUI
    typedef unsigned int uint32_t;
    typedef unsigned char uint8_t;

    #define __IO volatile
    #define __I volatile

    #define __ALIGN_BEGIN

#else
    #include <integer.h>
#endif

#define __RBIT
#define __CLZ

#define SCB_SCR_SLEEPONEXIT_Pos             1U                                            /*!< SCB SCR: SLEEPONEXIT Position */
#define SCB_SCR_SLEEPONEXIT_Msk            (1UL << SCB_SCR_SLEEPONEXIT_Pos)               /*!< SCB SCR: SLEEPONEXIT Mask */

#define SCB_SCR_SLEEPDEEP_Pos               2U                                            /*!< SCB SCR: SLEEPDEEP Position */
#define SCB_SCR_SLEEPDEEP_Msk              (1UL << SCB_SCR_SLEEPDEEP_Pos)                 /*!< SCB SCR: SLEEPDEEP Mask */


typedef struct
{
    uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
    uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
    uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
    uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
    uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
    uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
    uint8_t  SHP[12U];               /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
    uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
    uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
    uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
    uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
    uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
    uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
    uint32_t PFR[2U];                /*!< Offset: 0x040 (R/ )  Processor Feature Register */
    uint32_t DFR;                    /*!< Offset: 0x048 (R/ )  Debug Feature Register */
    uint32_t ADR;                    /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
    uint32_t MMFR[4U];               /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
    uint32_t ISAR[5U];               /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
    uint32_t RESERVED0[5U];
    uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
} SCB_Type;


#define SCB_BASE            (0)                                         /*!< System Control Block Base Address */

#define SCB                 ((SCB_Type       *)     SCB_BASE      )     /*!< SCB configuration struct */
