;
; DA_2_Assembly.asm
; Created 2/19/2022 2:26:46 PM
; Author : Dylan Cazares
; Task 1/2
;

.include <m328pbdef.inc>

.ORG 0			; location for reset
JMP MAIN		
.ORG 0x02		; location for external interrupt 0

MAIN:
	CBI DDRC, 3`	; setting PORTC.3 as an input
	SBI DDRB, 2		; setting PORTB.2 as an output
	SBI DDRB, 3		; setting PORTB.3 as an output
	

; stack initialization
LDI R20, HIGH(RAMEND)
OUT SPH, R20
LDI R20, LOW(RAMEND)
OUT SPL, R20

L1:
	SBIC PINC, 3				; skip if PINC.3 is cleared
	RJMP L1						; if PINC.3 cleared, check for input on PINC.3
	CBI PORTB, 2				; turn PORTB.2 LED on 
	; calling 250MS_Delay five times to generate a delay of 1.25 seconds
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms


	SBI PINB, 2					; turn PINB.2 LED off

DELAY_250ms:					; delay subroutine to generate a delay of 0.25 seconds
	LDI R17, 16					; R17 = 16
	L1_Delay: 
		LDI R18, 200			; R18 = 200
		L2_Delay: 
			LDI R19, 249		; R19 = 249
			NOP					;  No Operation (do nothing)
			NOP					;  No Operation (do nothing)
			L3_Delay:
				NOP				; No Operation (do nothing)
				NOP				; No Operation (do nothing)
				DEC R19			; decrement R19
				BRNE L3_Delay	; Branch to L3_Delay if R19 is not equal to 0
				DEC R18			; Decrement R18
				BRNE L2_Delay	; Branch to L2_Delay if R18 is not equal to 0
				DEC R17			; Decrement R17
				BRNE L1_Delay	; Branch to L1_Delay if R17 is not equal to 0
				RET				; return to caller
		
		
		
;
; DA_2_Assembly.asm
; Created 2/19/2022 2:26:46 PM
; Author : Dylan Cazares
; Task 3
;

.include <m328pbdef.inc>

.ORG 0			; location for reset
JMP MAIN		
.ORG 0x02		; location for external interrupt 0
JMP EX0_ISR		; jump to EX0_ISR

MAIN:
	CBI DDRC, 3`	; setting PORTC.3 as an input
	SBI DDRB, 2		; setting PORTB.2 as an output
	SBI DDRB, 3		; setting PORTB.3 as an output
	CBI DDRD, 2		; setting PORTD.2 as an input

; stack initialization
LDI R20, HIGH(RAMEND)
OUT SPH, R20
LDI R20, LOW(RAMEND)
OUT SPL, R20

L1:
	SBIC PINC, 3				; skip if PINC.3 is cleared
	RJMP L1						; if PINC.3 cleared, check for input on PINC.3
	CBI PORTB, 2				; turn PORTB.2 LED on 
	; calling 250MS_Delay five times to generate a delay of 1.25 seconds
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms
	CALL DELAY_250ms


LDI R20, 0x02					; make INT0 falling edge triggered
STS EICRA, R20					; store falling edge in EICRA
SBI DDRD, 1						; activating pull up
LDI R20, 1 << INT0				; enable INT0
OUT EIMSK, R20					; storeing R20 in EIMSK location
SEI								; enable interrupt

	SBI PINB, 2					; turn PINB.2 LED off

DELAY_250ms:					; delay subroutine to generate a delay of 0.25 seconds
	LDI R17, 16					; R17 = 16
	L1_Delay: 
		LDI R18, 200			; R18 = 200
		L2_Delay: 
			LDI R19, 249		; R19 = 249
			NOP					;  No Operation (do nothing)
			NOP					;  No Operation (do nothing)
			L3_Delay:
				NOP				; No Operation (do nothing)
				NOP				; No Operation (do nothing)
				DEC R19			; decrement R19
				BRNE L3_Delay	; Branch to L3_Delay if R19 is not equal to 0
				DEC R18			; Decrement R18
				BRNE L2_Delay	; Branch to L2_Delay if R18 is not equal to 0
				DEC R17			; Decrement R17
				BRNE L1_Delay	; Branch to L1_Delay if R17 is not equal to 0
				RET				; return to caller
EX0_ISR:
	L2: 
		SBIC PIND, 2			; skip if PIND.2 is cleared
		RJMP L2					; if PIND.2 is 0 skip to next
		CBI PORTB, 3			; Turn PINB.3 LED on
		CALL DELAY_250ms		; delay 250ms
		CALL DELAY_250ms		; delay 250ms
		SBI PINB, 2				; Turn PINB.3 LED off 
		RETI					; return from Interrupt