#include <avr/io.h>
#include <avr/interrupt.h>

void PWM_init(float duty) {
  TCCR3B  = 0;
  TCCR3C  = 0;
  TCNT3   = 0;
  
  // 0:in, 1:out, DDRB,DDRD: DIO, DDRC: AnalogIn
  // timer0->PD6,5, timer1->PB5,6, timer2->A...PE3, B...PE4, C...PE5
  //DDRB    = DDRB | B00000110;
  DDRE    = DDRE | B00110000; 

  // TCCR1A: COM1A1,COM1A0,COM1B1,COM1B0,-,-,WGM11,WGM10
  // TCCR1B: ICNC1,ICES1,-,WGM13,WGM12,CS12,CS11,CS10
  // COM1x1,COM1x0:   10: OCR1/TOP, 11:1-(OCR1/TOP)
  // WGM13,12,11,10:  1000(updown)
  // CS12,11,10: 000:stop, 001:clk, 010:clk/8, 011:clk/64,
  //             100:clk/256, 101:clk/1024
  TCCR3A  = (TCCR3A & B00000000) | bit(COM3B1) | bit(COM3C1) | bit(COM3C0);
  TCCR3B  = (TCCR3B & B11100000) | bit(WGM33) | bit(CS30);
  //TCCR3C  = (TCCR3C & B00000000);
  // | bit(FOC3B) | bit(FOC3C);

  // TCNT1: counter
  // TIMSK1: -,-,ICIE1,-,-,OCIE1B,OCIE1A,TOIE1
  TCNT3   = 0;
  TIMSK3  = bit(TOIE3);
  float deadtime = 100e-6f;

  // fpwm = fclk(16MHz)/(2(updown)*N*TOP), TOPmax=2^16
  // N*TOP = 16MHz/(2*1kHz)=8000, N=1, TOP=8000
  ICR3    = 32000;
  OCR3B   = ICR3 * duty;
  OCR3C   = OCR3B + deadtime * 16e+6f;// deadtime 0.5us -> 0.5us*16MHz=8
}                     // deadtime 100us -> 100us * 16MHz = 1600     

// TIMER1_COMPB_vect, TIMER1_COMPA_vect, TIMER1_OVF_vect
ISR (TIMER3_OVF_vect) {
  
}

void setup() {
  // put your setup code here, to run once:
  noInterrupts();  
  PWM_init(0.9);
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
}
