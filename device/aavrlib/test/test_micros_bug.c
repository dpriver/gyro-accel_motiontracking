#include "systick.h"
#include "delay.h"
#include "common.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usart.h"

const uint32_t INTERVALE = 800;
const uint32_t NUM_VALUES = 100;

volatile uint32_t micro[100];
volatile uint32_t mili[100];
volatile uint32_t diff[100];
volatile uint8_t fails[100];
volatile uint8_t fails_post[100];
volatile uint32_t indice;
volatile uint32_t num_fails;
volatile uint32_t last_time;

enum{ADC0=0x00, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7};

void config_adc(uint8_t port){
	ADMUX = _BV(REFS0) | port;
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	ADCSRB = 0x00;
	DIDR0 = 0x00;
}

uint16_t readAnalog(){
	uint8_t low, high;
	ADCSRA |= _BV(ADSC);
	while( (ADCSRA & _BV(ADSC)) );
	low = ADCL;
	high = ADCH;
	return (uint16_t) ((high << 8) | low);
}


uint32_t micros_prueba(){
	cli();
	micro[indice] = TCNT1 >> 1;
	mili[indice] = system_tick.current_tick;
	//if( micro[indice] < 32 ){
	if( (TIFR1 & _BV(OCF1A)) && (micro[indice] == 0) ){
		mili[indice]++;
        IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_8));
		//DIGITAL_PORT_B |= _BV(DIGITAL_8);
		//fails[indice] = TRUE;
		//num_fails++;
	}
	sei();
	return (uint32_t)mili[indice]*1000 + (uint32_t)micro[indice];
}



void initialize(){
	cli();
	//DIGITAL_PORT_B |= _BV(DIGITAL_9);
    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_9));
	delay_us(200);
	for(uint8_t i = 0; i < NUM_VALUES ; i++){
		micro[i] = 0;
		mili[i] = 0;
		diff[i] = 0;
		fails[i] = FALSE;
		fails_post[i] = FALSE;
	}
	num_fails = 0;
	indice = 0;
	last_time = 0;
	//DIGITAL_PORT_B &= ~_BV(DIGITAL_9);
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_9));
	sei();
}

void dump(){
	usart_print("\n========== DUMP ============\n");
	for(uint32_t i = 0; i < NUM_VALUES ; i++){
		usart_print("\nvalue ");
		usart_printnumber32(i);
		usart_print(": ");
		usart_printnumber32(micro[i]);
		usart_print(": ");
		usart_printnumber32(mili[i]);
		usart_print(" --> [");
		usart_printnumber32(diff[i]);
		usart_print("]");
		if(fails[i])
			usart_print("  FAIL");
		if(fails_post[i])
			usart_print("  FAIL_POST");
	}
	usart_print("\nNUM. FAILS: ");
	usart_printnumber32(num_fails);
	usart_print("Tiempo ISR: ");
	usart_printnumber32((uint32_t)system_tick.tiempoISR);
	usart_print("\n============================\n");
}

int main(void){
	uint32_t micro_aux;

	init_system_tick();
	usart_init();
	config_adc(ADC0);

	sei();

	//DIGITAL_CONF_REG_B |= _BV(DIGITAL_8) | _BV(DIGITAL_9);
    IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_8) | _BV(PIN_9));

	//DIGITAL_PORT_B |= _BV(DIGITAL_8);
    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_8));
	delay_ms(500);
	//DIGITAL_PORT_B &= ~_BV(DIGITAL_8);
	IOPORT_VALUE(LOW, PORT_B, _BV(PIN_8));
    delay_ms(500);

	//DIGITAL_PORT_B |= _BV(DIGITAL_9);
    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_9));
	while(readAnalog() <= 10);
	//DIGITAL_PORT_B &= ~_BV(DIGITAL_9);
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_9));

// llama a la funcion micros() cada INTERVALO segundo
	while(TRUE){
		do{
			initialize();
			while(indice < NUM_VALUES){
				delay_us(INTERVALE);
				micro_aux = micros_prueba();
				diff[indice] = micro_aux - last_time;
				if(  ((diff[indice] < (INTERVALE+220)*0.9) || (diff[indice] > (INTERVALE+220)*1.1)) && (indice > 0) ){
					fails_post[indice] = TRUE;
					if(!fails[indice])
						num_fails++;
					//DIGITAL_PORT_B |= _BV(DIGITAL_8);
                    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_8));
				}
				last_time = micro_aux;
				indice++;
			}
		}while( (num_fails == 0) );
		dump();

		while(readAnalog() > 10);
		//DIGITAL_PORT_B &= ~_BV(DIGITAL_8);
        IOPORT_VALUE(LOW, PORT_B, _BV(PIN_8));
	}

}
