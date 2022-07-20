;
; Design Assignment 1.asm
;
; Created: 2/11/2022 6:58:13 PM
; Author : Dylan Cazares
;
.ORG $0 ; starting at 0

; Question 1: Storing 16-bit number 0x1234 in SRAM location 0x402
LDI r16, 0x12 ; r16 = 0x12 upper bytes
LDI r17, 0x34 ; r17 = 0x34 lower bytes
STS 0x402, r16 ; storing r16 into data 0x402
STS 0x403, r17 ; storing r17 into data 0x403

; Question 2: Storing 16-bit number 0x5678 in SRAM location 0x410
LDI r26, 0x56 ; r16 = 0x56 upper bytes
LDI r27, 0x78 ; r17 = 0x78 lower bytes
STS 0x410, r26 ; storing r16 into data 0x402
STS 0x411, r27 ; storing r17 into data 0x403

; Question 3: Sum of two numbers stored in EEPROM starting location
ADD r17, r27 ; r17 = r17 + r27 = 0x34 + 0x78 = 0xAC adds lower bytes
ADC r16, r26 ; r16 = r16 + r26 = 0x12 + 0x56 = 0x68 adds upper bytes

LDI R20,HIGH(RAMEND) ; initializing the stack
OUT SPH,R20
LDI R20,LOW(RAMEND) 
OUT SPL,R20

LDI XH,HIGH(0x40) ;XH POINTER HIGH VALUE 00
LDI XL,LOW(0x40) ;XL POINTER LOW VALUE 00

CALL STORE_IN_EEPROM ;XH POINTER HIGH VALUE 00
INC XL ;increment XL
MOV r21, r17 ;Copy value in R21 into R17

CALL STORE_IN_EEPROM
INC XL ;increment XL
MOV r22, r16 ;Copy value in R16 into R22

STORE_IN_EEPROM:
SBIC EECR, EEPE ;wait for write process to complete
RJMP STORE_IN_EEPROM ;wait for write process to complete
OUT EEARH,XH ;set the value of the address
OUT EEARL,XL ;set the value of the address
OUT EEDR,r17 ;R17 goes to the data register
SBI EECR,EEMPE ;master is enabled
SBI EECR,EEPE ;protection is enabled 
RET

; Question 4: Storing 10 16-bit numbers from 0x0910 at Program Memory location and 
; retrieving them to 0x500 SRAM location using X pointer. Sum the 10 numbers and 
; store them in SRAM location 0x406