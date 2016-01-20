#pragma once
#include <stdint.h>

#define enableInterrupts()    __asm__("rim\n")
#define waitforinterrupt()    __asm__("wfi\n")

#define ODRREG(base) (base)
#define IDRREG(base) (base + 1)
#define DDRREG(base) (base + 2)
#define CR1REG(base) (base + 3)
#define CR2REG(base) (base + 4)

#define PA_BASE 0x5000
#define PB_BASE 0x5005
#define PC_BASE 0x500A
#define PD_BASE 0x500F
#define PE_BASE 0x5014
#define PF_BASE 0x5019

#define PA_ODR (volatile uint8_t*)(ODRREG(PA_BASE))
#define PA_DDR (volatile uint8_t*)(DDRREG(PA_BASE))
#define PA_CR1 (volatile uint8_t*)(CR1REG(PA_BASE))
#define PA_CR2 (volatile uint8_t*)(CR2REG(PA_BASE))

#define PB_ODR (volatile uint8_t*)(ODRREG(PB_BASE))
#define PB_IDR (volatile uint8_t*)(IDRREG(PB_BASE))
#define PB_DDR (volatile uint8_t*)(DDRREG(PB_BASE))
#define PB_CR1 (volatile uint8_t*)(CR1REG(PB_BASE))
#define PB_CR2 (volatile uint8_t*)(CR2REG(PB_BASE))

#define PC_ODR (volatile uint8_t*)(ODRREG(PC_BASE))
#define PC_DDR (volatile uint8_t*)(DDRREG(PC_BASE))
#define PC_CR1 (volatile uint8_t*)(CR1REG(PC_BASE))
#define PC_CR2 (volatile uint8_t*)(CR2REG(PC_BASE))

#define PD_ODR (volatile uint8_t*)(ODRREG(PD_BASE))
#define PD_IDR (volatile uint8_t*)(IDRREG(PD_BASE))
#define PD_DDR (volatile uint8_t*)(DDRREG(PD_BASE))
#define PD_CR1 (volatile uint8_t*)(CR1REG(PD_BASE))
#define PD_CR2 (volatile uint8_t*)(CR2REG(PD_BASE))

#define PE_ODR (volatile uint8_t*)(ODRREG(PE_BASE))
#define PE_DDR (volatile uint8_t*)(DDRREG(PE_BASE))
#define PE_CR1 (volatile uint8_t*)(CR1REG(PE_BASE))
#define PE_CR2 (volatile uint8_t*)(CR2REG(PE_BASE))

#define PF_ODR (volatile uint8_t*)(ODRREG(PF_BASE))
#define PF_DDR (volatile uint8_t*)(DDRREG(PF_BASE))
#define PF_CR1 (volatile uint8_t*)(CR1REG(PF_BASE))
#define PF_CR2 (volatile uint8_t*)(CR2REG(PF_BASE))

#define UART2_BASE		0x5240
#define UART2_SR		(*(volatile uint8_t*)(UART2_BASE))
#define UART2_SR_TXE	(1 << 7)
#define UART2_SR_TC		(1 << 6)

#define UART2_DR	(*(volatile uint8_t*)(UART2_BASE + 0x1))
#define UART2_BRR1	(*(volatile uint8_t*)(UART2_BASE + 0x2))
#define UART2_BRR2	(*(volatile uint8_t*)(UART2_BASE + 0x3))
#define UART2_CR1	(*(volatile uint8_t*)(UART2_BASE + 0x4))

#define UART2_CR2		(*(volatile uint8_t*)(UART2_BASE + 0x5))
#define UART2_CR2_TIEN	(1 << 7)
#define UART2_CR2_TCIEN	(1 << 6)
#define UART2_CR2_RIEN	(1 << 5)
#define UART2_CR2_TEN	(1 << 3)
#define UART2_CR2_REN	(1 << 2)

#define UART2_CR3	(*(volatile uint8_t*)(UART2_BASE + 0x6))
#define UART2_CR4	(*(volatile uint8_t*)(UART2_BASE + 0x7))
#define UART2_CR5	(*(volatile uint8_t*)(UART2_BASE + 0x8))
#define UART2_CR6	(*(volatile uint8_t*)(UART2_BASE + 0x9))
#define UART2_GTR	(*(volatile uint8_t*)(UART2_BASE + 0xa))
#define UART2_PSCR	(*(volatile uint8_t*)(UART2_BASE + 0xb))

#define ADC_DBBASE	0x53E0
#define ADC_DB0R	(volatile uint8_t*)(ADC_DBBASE)
#define ADC_DB1R	(volatile uint8_t*)(ADC_DBBASE + 1)
#define ADC_DB2R	(volatile uint8_t*)(ADC_DBBASE + 2)
#define ADC_DB3R	(volatile uint8_t*)(ADC_DBBASE + 3)

#define ADC_BASE	0x5400
#define ADC_CSR		(*(volatile uint8_t*)(ADC_BASE))
#define ADC_CSR_EOC	(1 << 7)
#define ADC_CR1		(*(volatile uint8_t*)(ADC_BASE + 1))
#define ADC_CR1_ADON	(1)
#define ADC_CR1_CONT	(1 << 1)

#define ADC_CR2		(volatile uint8_t*)(ADC_BASE + 2)
#define ADC_CR3		(volatile uint8_t*)(ADC_BASE + 3)
#define ADC_DRH		(*(volatile uint8_t*)(ADC_BASE + 4))
#define ADC_DRL		(*(volatile uint8_t*)(ADC_BASE + 5))
#define ADC_TDRH	(volatile uint8_t*)(ADC_BASE + 6)
#define ADC_TDRL	(volatile uint8_t*)(ADC_BASE + 7)
#define ADC_HTRH	(volatile uint8_t*)(ADC_BASE + 8)
#define ADC_HTRL	(volatile uint8_t*)(ADC_BASE + 9)
#define ADC_LTRH	(volatile uint8_t*)(ADC_BASE + a)
#define ADC_LTRL	(volatile uint8_t*)(ADC_BASE + b)
#define ADC_AWSRH	(volatile uint8_t*)(ADC_BASE + c)
#define ADC_AWSRL	(volatile uint8_t*)(ADC_BASE + d)
#define ADC_AWCRH	(volatile uint8_t*)(ADC_BASE + e)
#define ADC_AWCRL	(volatile uint8_t*)(ADC_BASE + f)

#define CLK_BASE	0x50C0
#define CLK_ICKR	(volatile uint8_t*)(CLK_BASE)
#define CLK_ECKR        (volatile uint8_t*)(CLK_BASE + 1)
#define CLK_CMSR        (volatile uint8_t*)(CLK_BASE + 3)
#define CLK_SWR		(volatile uint8_t*)(CLK_BASE + 4)
#define CLK_SWRCR       (volatile uint8_t*)(CLK_BASE + 5)
#define CLK_CKDIVR	(volatile uint8_t*)(CLK_BASE + 6)
#define CLK_PCKENR1	(volatile uint8_t*)(CLK_BASE + 7)
#define CLK_CSSR        (volatile uint8_t*)(CLK_BASE + 8)
#define CLK_CCOR        (volatile uint8_t*)(CLK_BASE + 9)
#define CLK_PCKENR2	(volatile uint8_t*)(CLK_BASE + 0xa)
#define CLK_HSITRIMR	(volatile uint8_t*)(CLK_BASE + 0xc)
#define CLK_SWIMCCR	(volatile uint8_t*)(CLK_BASE + 0xd)

#define TIM1_BASE		0x5250
#define TIM1_CR1		(*(volatile uint8_t*)(TIM1_BASE))
#define TIM1_CR1_ARPE	(1 << 7)
#define TIM1_CR1_CEN	1

#define TIM1_CR2	(*(volatile uint8_t*)(TIM1_BASE + 0x1))
#define TIM1_SMCR	(*(volatile uint8_t*)(TIM1_BASE + 0x2))
#define TIM1_ETR	(*(volatile uint8_t*)(TIM1_BASE + 0x3))
#define TIM1_IER	(*(volatile uint8_t*)(TIM1_BASE + 0x4))
#define TIM1_SR1	(*(volatile uint8_t*)(TIM1_BASE + 0x5))
#define TIM1_SR2	(*(volatile uint8_t*)(TIM1_BASE + 0x6))

#define TIM1_EGR	(*(volatile uint8_t*)(TIM1_BASE + 0x7))
#define TIM1_EGR_UG	1

#define TIM1_CCMR1	(*(volatile uint8_t*)(TIM1_BASE + 0x8))
#define TIM1_CCMR2	(*(volatile uint8_t*)(TIM1_BASE + 0x9))
#define TIM1_CCMR3	(*(volatile uint8_t*)(TIM1_BASE + 0xa))
#define TIM1_CCMR4	(*(volatile uint8_t*)(TIM1_BASE + 0xb))

#define TIM1_CCER1		(*(volatile uint8_t*)(TIM1_BASE + 0xc))
#define TIM1_CCER1_CC1E	1

#define TIM1_CCER2	(*(volatile uint8_t*)(TIM1_BASE + 0xd))
#define TIM1_CNTRH	(*(volatile uint8_t*)(TIM1_BASE + 0xe))
#define TIM1_CNTRL	(*(volatile uint8_t*)(TIM1_BASE + 0xf))
#define TIM1_PSCRH	(*(volatile uint8_t*)(TIM1_BASE + 0x10))
#define TIM1_PSCRL	(*(volatile uint8_t*)(TIM1_BASE + 0x11))
#define TIM1_ARRH	(*(volatile uint8_t*)(TIM1_BASE + 0x12))
#define TIM1_ARRL	(*(volatile uint8_t*)(TIM1_BASE + 0x13))
#define TIM1_RCR	(*(volatile uint8_t*)(TIM1_BASE + 0x14))
#define TIM1_CCR1H	(*(volatile uint8_t*)(TIM1_BASE + 0x15))
#define TIM1_CCR1L	(*(volatile uint8_t*)(TIM1_BASE + 0x16))
#define TIM1_CCR2H	(*(volatile uint8_t*)(TIM1_BASE + 0x17))
#define TIM1_CCR2L	(*(volatile uint8_t*)(TIM1_BASE + 0x18))
#define TIM1_CCR3H	(*(volatile uint8_t*)(TIM1_BASE + 0x19))
#define TIM1_CCR3L	(*(volatile uint8_t*)(TIM1_BASE + 0x1a))
#define TIM1_CCR4H	(*(volatile uint8_t*)(TIM1_BASE + 0x1b))
#define TIM1_CCR4L	(*(volatile uint8_t*)(TIM1_BASE + 0x1c))

#define TIM1_BKR		(*(volatile uint8_t*)(TIM1_BASE + 0x1d))
#define TIM1_BKR_MOE	(1 << 7)

#define TIM1_DTR	(*(volatile uint8_t*)(TIM1_BASE + 0x1e))
#define TIM1_OISR	(*(volatile uint8_t*)(TIM1_BASE + 0x1f))

#define INTERRUPT_UART2_TXCOMPLETE	20
#define INTERRUPT_UART2_RXFULL		21
