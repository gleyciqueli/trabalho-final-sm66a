#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

// ---------------- DEFINIÇÕES ----------------
#define LED_RED     0x02  // PF1
#define LED_GREEN   0x08  // PF3
#define LED_BLUE    0x10  // PE4
#define LED_YELLOW  0x02  // PE1

#define BTN_MASK    0x0F  // PD0–PD3
#define MAX_SEQ     10

uint8_t sequence[MAX_SEQ] = {0, 2, 1, 3, 0, 1, 2, 3, 0, 3};
uint8_t level = 0;

// ---------------- PROTÓTIPOS ----------------
void Clock_Init(uint32_t sysdiv);
void PortF_Init(void);
void PortE_Init(void);
void PortD_Init(void);
void Timer0A_Init(uint32_t period);
void Delay_t(void);

void LED_On(uint8_t index);
void LED_Off(void);
void FlashAllLeds(uint8_t times);
uint8_t GetButton(void);
void PlaySequence(void);
bool CheckUserInput(void);

// ---------------- FUNÇÃO PRINCIPAL ----------------
int main(void) {
    Clock_Init(4);          // Clock = 80 MHz
    PortF_Init();           // LEDs PF
    PortE_Init();           // LEDs PE
    PortD_Init();           // Botões PD
    Timer0A_Init(60000);    // Atraso ~500 ms


    while (1) {
        FlashAllLeds(3);        // Pisca sempre que reinicia o jogo
        level = 0;

        while (1) {
            PlaySequence();

            if (!CheckUserInput()) {
                FlashAllLeds(3);  // Pisca no erro
                break;            // Reinicia do zero
            } else {
                level++;
                if (level >= MAX_SEQ) {
                    FlashAllLeds(3);  // Final da sequência
                    break;            // Reinicia
                }
            }

            Delay_t();  // Pausa entre rodadas
        }
    }
}

// ---------------- GPIO ----------------

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;
    volatile int delay = SYSCTL_RCGCGPIO_R;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_DIR_R |= (LED_RED | LED_GREEN);
    GPIO_PORTF_DEN_R |= (LED_RED | LED_GREEN);
    GPIO_PORTF_AMSEL_R = 0;
    GPIO_PORTF_PCTL_R = 0;
    GPIO_PORTF_AFSEL_R = 0;
}

void PortE_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x10;
    while ((SYSCTL_PRGPIO_R & 0x10) == 0);
    GPIO_PORTE_DIR_R |= (LED_BLUE | LED_YELLOW);
    GPIO_PORTE_DEN_R |= (LED_BLUE | LED_YELLOW);
    GPIO_PORTE_AMSEL_R = 0;
    GPIO_PORTE_PCTL_R = 0;
    GPIO_PORTE_AFSEL_R = 0;
}

void PortD_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x08;
    while ((SYSCTL_PRGPIO_R & 0x08) == 0);
    GPIO_PORTD_DIR_R &= ~BTN_MASK;
    GPIO_PORTD_DEN_R |= BTN_MASK;
    GPIO_PORTD_PUR_R |= BTN_MASK;
    GPIO_PORTD_AMSEL_R = 0;
    GPIO_PORTD_PCTL_R = 0;
    GPIO_PORTD_AFSEL_R = 0;
}

// ---------------- TIMER E CLOCK ----------------

void Clock_Init(uint32_t sysdiv) {
    SYSCTL_RCC2_R |= 0x80000000;
    SYSCTL_RCC2_R |= 0x00000800;
    SYSCTL_RCC_R &= ~0x00400000;
    SYSCTL_RCC_R &= ~0x000007C0;
    SYSCTL_RCC_R += 0x00000540;
    SYSCTL_RCC2_R &= ~0x00000070;
    SYSCTL_RCC2_R &= ~0x00002000;
    SYSCTL_RCC2_R |= 0x40000000;
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (sysdiv << 22);
    SYSCTL_RCC_R |= 0x00400000;
    while ((SYSCTL_RIS_R & 0x00000040) == 0);
    SYSCTL_RCC2_R &= ~0x00000800;
}

void Timer0A_Init(uint32_t period) {
    SYSCTL_RCGCTIMER_R |= 0x01;
    TIMER0_CTL_R = 0x00;
    TIMER0_CFG_R = 0x04;             // Timer 16 bits
    TIMER0_TAMR_R = 0x01;            // One-shot
    TIMER0_TAILR_R = period - 1;
    TIMER0_TAPR_R = 199;             // Prescaler (~500ms)
}

void Delay_t(void) {
    // Primeiro, desabilitamos o timer para garantir que ele não esteja contando
    TIMER0_CTL_R &= ~0x01; // Desabilita o timer A
    // Recarregamos o valor inicial do contador (TAILR)
    TIMER0_TAV_R = TIMER0_TAILR_R; // Define o valor atual do timer para o valor de recarga
    // Habilitamos o timer para iniciar a contagem regressiva
    TIMER0_CTL_R |= 0x01;  // Habilita o timer A
    // Espera o timer atingir 0 (ou seja, a contagem terminar)
    while ((TIMER0_RIS_R & 0x01) == 0); // Espera a flag de timeout (TATORIS)
    TIMER0_ICR_R |= 0x01; // Limpa a flag de timeout para a próxima vez
}
// ---------------- LÓGICA DO JOGO ----------------

void LED_On(uint8_t index) {
    switch (index) {
        case 0: GPIO_PORTF_DATA_R |= LED_RED; break;
        case 1: GPIO_PORTE_DATA_R |= LED_BLUE; break;
        case 2: GPIO_PORTF_DATA_R |= LED_GREEN; break;
        case 3: GPIO_PORTE_DATA_R |= LED_YELLOW; break;
    }
}

void LED_Off(void) {
    GPIO_PORTF_DATA_R &= ~(LED_RED | LED_GREEN);
    GPIO_PORTE_DATA_R &= ~(LED_BLUE | LED_YELLOW);
}

void FlashAllLeds(uint8_t times) {
    uint8_t i;
    for (i = 0; i < times; i++) {
        GPIO_PORTF_DATA_R |= (LED_RED | LED_GREEN);
        GPIO_PORTE_DATA_R |= (LED_BLUE | LED_YELLOW);
        Delay_t();
        GPIO_PORTF_DATA_R &= ~(LED_RED | LED_GREEN);
        GPIO_PORTE_DATA_R &= ~(LED_BLUE | LED_YELLOW);
        Delay_t();
    }
}

uint8_t GetButton(void) {
    uint8_t val;
    while (1) {
        val = GPIO_PORTD_DATA_R & BTN_MASK;
        if (val != BTN_MASK) {
            Delay_t();  // debounce
            val = GPIO_PORTD_DATA_R & BTN_MASK;
            if (val == 0x0E) return 0; // PD0
            if (val == 0x0D) return 1; // PD1
            if (val == 0x0B) return 2; // PD2
            if (val == 0x07) return 3; // PD3
        }
    }
}

void PlaySequence(void) {
    uint8_t i;
    for (i = 0; i <= level; i++) {
        LED_On(sequence[i]);
        Delay_t();
        LED_Off();
        Delay_t();
    }
}

bool CheckUserInput(void) {
    uint8_t i;
    for (i = 0; i <= level; i++) {
        uint8_t btn = GetButton();
        LED_On(btn);
        Delay_t();
        LED_Off();
        if (btn != sequence[i]) {
            return false;
        }
    }
    return true;
}
