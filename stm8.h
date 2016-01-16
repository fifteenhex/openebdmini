#include <stdint.h>

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


#define UART2_BASE	0x5240
#define UART2_SR	(volatile uint8_t*)( UART_BASE)
#define UART2_DR	(volatile uint8_t*)( UART_BASE + 1)
#define UART2_BRR1	(volatile uint8_t*)( UART_BASE + 2)
#define UART2_BRR2	(volatile uint8_t*)( UART_BASE + 3)
#define UART2_CR1	(volatile uint8_t*)( UART_BASE + 4)
#define UART2_CR2	(volatile uint8_t*)( UART_BASE + 5)
#define UART2_CR3	(volatile uint8_t*)( UART_BASE + 6)
#define UART2_CR4	(volatile uint8_t*)( UART_BASE + 7)
#define UART2_CR5	(volatile uint8_t*)( UART_BASE + 8)
#define UART2_CR6	(volatile uint8_t*)( UART_BASE + 9)
#define UART2_GTR	(volatile uint8_t*)( UART_BASE + a)
#define UART2_PSCR	(volatile uint8_t*)( UART_BASE + b)

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
#define CLK_PCKENR2	(volatile uint8_t*)(CLK_BASE + a)
#define CLK_HSITRIMR	(volatile uint8_t*)(CLK_BASE + c)
#define CLK_SWIMCCR	(volatile uint8_t*)(CLK_BASE + d)
