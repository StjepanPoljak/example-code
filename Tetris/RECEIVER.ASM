LIST p=PIC16F627
;#include "p16F627.inc"

;Philips RC5 receiver protokol za PIC16F627(A)

;Konfiguracija
;Opcija				Vrij.	Objasnjenje
;-----------------------------------------------------------------
;Code protection		1111	(OFF)
;Unimplemented			1
;Data code protection		1 	(OFF)
;Low voltage programming	0	(OFF)
;BOD Reset			1	(ON)
;MCLR				0	(MCLR pin OFF)
;Power-up Timer			0	(ON)
;Watchdog Timer			0	(OFF)
;Oscillator			001	(INTRC oscillator)

__CONFIG B'11111101000001'

;Deklaracije registara
;-----------------------------------------------------------------

W 		EQU	00h
F		EQU	01h
STATUS		EQU	03h
RP0		EQU	05h
PORTA		EQU	05h
PORTB		EQU	06h
CMCON		EQU	1Fh
TRISA		EQU	85h
TRISB		EQU	86h
TMR1L		EQU	0Eh
TMR1H		EQU	0Fh
T1CON		EQU	10h

PIE1		EQU	8Ch
PIR1		EQU	0Ch

Z		EQU	02h
C		EQU	00h

;Deklaracije pinova
;-----------------------------------------------------------------

RA0		EQU	00h
RA1		EQU	01h
RA2		EQU	02h
RA3		EQU	03h
RA4		EQU	04h

RB0		EQU	00h
RB1		EQU	01h
RB2		EQU	02h
RB3		EQU	03h
RB4		EQU	04h
RB5		EQU	05h
RB6		EQU	06h
RB7		EQU	07h

;Deklaracije varijabli
;-----------------------------------------------------------------

PORTASTATUS		EQU	20h
PORTBSTATUS		EQU	21h

PREVIOUS		EQU	22h
CURRENT			EQU	23h

TEMP			EQU	24h
COUNTER			EQU	25h

PARTONE			EQU	26h
PARTTWO			EQU	27h

PLACE			EQU	28h
FIRST			EQU	29h
START			EQU	2Ah

;Inicijalizacija programa
;-----------------------------------------------------------------

ORG 00h		;pocetak programa

;Iskljucujemo analogni input za port A
MOVLW B'00000111'
MOVWF CMCON

;Prelazimo na bank 1
BSF STATUS, RP0

;Postavljamo RA0 (signal) i RA1 (error checking) za input.
MOVLW 03h
MOVWF TRISA

;Postavljamo port B za output.
MOVLW 00h
MOVWF TRISB

;Vracamo se na bank 0
BCF STATUS, RP0

CLRF PORTBSTATUS
CLRF PORTB
CLRF PORTASTATUS
CLRF PORTA

MOVLW B'00000101'		;postavljamo postavke za TIMER1
MOVWF T1CON

LStart

	MOVLW 01h
	MOVWF PREVIOUS		;postavljamo prethodni na 01h

	MOVLW 01h
	MOVWF CURRENT		;postavljamo trenutni na 01h

	CLRF TMR1L		;resetiramo timer
	CLRF TMR1H
	MOVLW D'14'		;postavljamo COUNTER na 14
	MOVWF COUNTER

	CLRF PARTTWO		;brisemo prethodne podatke
	CLRF PARTONE
	CLRF PLACE

	MOVLW 01h		;na pocetku smo
	MOVWF START

;Glavna petlja
;-----------------------------------------------------------------

LMain

	CALL MC_IsStateChanged		;je li stanje promijenjeno?
	MOVWF TEMP
	BTFSS TEMP, 00h
	GOTO LContinue			;ako nije, nastavljamo

	BTFSC START, 00h		;stanje je promijenjeno,
	GOTO LFirstPlace		;ako smo na pocetku uzorkovanja
					;idemo na LFirstPlace
	MOVF TMR1H, W
	MOVWF TEMP
	MOVLW B'00000100'		;ako je vrijeme manje od 1024
	CALL MC_LessThan
	MOVWF TEMP
	BTFSC TEMP, 00h
	GOTO LHalf			;idemo na LHalf

	MOVF COUNTER, W			;ako je vrijeme vece od 1024
	MOVWF TEMP			;provjeravamo jesmo li na prvom
	MOVLW D'7'			;ili na drugom dijelu
	CALL MC_LessThan
	MOVWF TEMP
	BTFSC TEMP, 00h
	GOTO LStillOnPARTONEWhole	;jos smo na PARTONE, ali imamo vrijeme od 2048

	RLF PARTTWO, F			;sada smo na PARTTWO, vrijeme od 2048
	BCF PARTTWO, 00h		;pomicemo PARTTWO i dodajemo mu trenutnu
	MOVF CURRENT, W			;vrijednost
	ADDWF PARTTWO, F

	GOTO LDecreaseCountAndCheckIfDone

LStillOnPARTONEWhole	

	RLF PARTONE, F			;imamo dvije faze preskocene
	BCF PARTONE, 00h		;na prvom smo dijelu, vrijeme od 2048
	MOVF CURRENT, W
	ADDWF PARTONE, F		;dodajemo trenutnu vrijednost
	GOTO LDecreaseCountAndCheckIfDone

LHalf

	BTFSC PLACE, 00h		;na prvoj smo polovici, gledamo li prvi dio?
	GOTO LSecondPlace		;gledamo drugi dio

LFirstPlace

	CLRF START			;brisemo START, sigurno vise nismo na pocetku
	MOVF CURRENT, W			;gledamo prvi dio, prebacimo ga u FIRST
	MOVWF FIRST
	COMF PLACE, F			;stavimo PLACE na FFh

	CLRF TMR1L			;resetiramo timer
	CLRF TMR1H

	GOTO LContinue			;nastavljamo...

LSecondPlace

	CLRF PLACE			;na drugom smo mjestu,
					;postavljamo PLACE na 00h
	MOVF COUNTER, W
	MOVWF TEMP
	MOVLW D'7'			;ovo treba jos testirati!!!
	CALL MC_LessThan		;gledamo jesmo li na prvom ili na drugom dijelu
	MOVWF TEMP
	BTFSC TEMP, 00h
	GOTO LStillOnPARTONE		;jos smo na prvom...

	RLF PARTTWO, F			;na drugom smo dijelu, dodajemo vrijednost
	BCF PARTTWO, 00h		;u PARTTWO
	MOVF CURRENT, W
	ADDWF PARTTWO, F

	GOTO LDecreaseCountAndCheckIfDone

LStillOnPARTONE

	RLF PARTONE, F			;na prvom smo dijelu, dodajemo
	BCF PARTONE, 00h		;trenutnu vrijednost u PARTONE
	MOVF CURRENT, W
	ADDWF PARTONE, F

LDecreaseCountAndCheckIfDone

	CLRF TMR1L			;resetiramo timer
	CLRF TMR1H

	DECFSZ COUNTER, F		;smanjujemo COUNTER za 1
	GOTO LContinue			;ako je COUNTER>0 nastavljamo
	GOTO LDone			;ako je COUNTER=0 gotovi smo

LContinue

	CALL MC_Iterate			;obavljamo iterativni postupak

	MOVF TMR1H, W			;provjeravamo je li isteklo 2048us
	MOVWF TEMP
	MOVLW B'00001000'
	CALL MC_LessThan
	MOVWF TEMP
	BTFSS TEMP, 00h
	GOTO LCheckCount		;isteklo je, ali cemo provjeriti COUNTER

	GOTO LMain			;vrijeme nije isteklo, nastavljamo s petljom

LCheckCount

	MOVLW 01h			;provjeravamo je li COUNTER=1
	XORWF COUNTER, W
	
	BTFSS STATUS, Z
	GOTO LStart			;COUNTER!=1, pa idemo ispocetka;
					;vjerojatno je bila rijec o smetnji
	RLF PARTONE, F			;COUNTER=1, pa popravimo onaj bug...
	BCF PARTONE, 00h		;pomicemo PARTONE u lijevo...

	GOTO LDone			;gotovi smo, fiju!

LDone

	BTFSS PARTTWO, 07h		;provjeravamo bitove...
	GOTO LStart			;Error!	

	;za video
	BTFSC PARTTWO, 04h
	GOTO LStart			;Error!	
	BTFSC PARTTWO, 03h
	GOTO LStart			;Error!	
	BTFSS PARTTWO, 02h
	GOTO LStart			;Error!	
	BTFSC PARTTWO, 01h
	GOTO LStart			;Error!	
	BTFSS PARTTWO, 00h
	GOTO LStart			;Error!	

	BCF PARTONE, 07h
	BCF PARTONE, 06h

	MOVLW B'00010000'		;je li tipka za gore?
	XORWF PARTONE, W

	MOVLW B'00000001'		;stavimo kod 01h
	BTFSC STATUS, Z
	GOTO LLoadMe

	MOVLW B'00010001'		;je li tipka za dolje?
	XORWF PARTONE, W

	MOVLW B'00000010'		;stavimo kod 02h
	BTFSC STATUS, Z
	GOTO LLoadMe

	MOVLW B'00010101'		;je li tipka za lijevo?
	XORWF PARTONE, W

	MOVLW B'00000011'		;stavimo kod 03h
	BTFSC STATUS, Z
	GOTO LLoadMe

	MOVLW B'00010110'		;je li tipka za desno?
	XORWF PARTONE, W

	MOVLW B'00000100'		;stavimo kod 04h
	BTFSC STATUS, Z
	GOTO LLoadMe

	MOVLW B'00010111'		;je li tipka za akciju?
	XORWF PARTONE, W

	MOVLW B'00000101'		;stavimo kod REV(05h)
	BTFSC STATUS, Z
	GOTO LLoadMe

	GOTO LStart			;nije bilo nista!!

LLoadMe

	MOVWF PARTONE
	MOVF PARTONE, W
	CALL MC_LoadToPORTB

	BSF PORTBSTATUS, 06h		;saljemo impuls da imamo novi podatak
	MOVF PORTBSTATUS, W		;impuls traje 2-3us
	MOVWF PORTB

	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	BCF PORTBSTATUS, 06h
	MOVF PORTBSTATUS, W
	MOVWF PORTB

	GOTO LStart			;idemo ispocetka, sve...

;Microcontroller PORT funkcije
;-----------------------------------------------------------------

MC_LoadFromPORTA
;Ucitava vrijednosti iz PORTA i stavlja u PORTASTATUS i W

	MOVF PORTA, W
	MOVWF PORTASTATUS
	NOP

RETURN

MC_LoadToPORTB
;iz W registra ucitava u PORTBSTATUS i kopira to isto u PORTB

	MOVWF PORTBSTATUS
	MOVWF PORTB
	NOP

RETURN

MC_LessThan
;W je 1 ako je TEMP manji od W

	SUBWF TEMP, W

	BTFSC STATUS, C
	RETLW 00h
	RETLW 01h

RETURN

MC_LoadInputToW
;W je signal iz RA0

	CALL MC_LoadFromPORTA
	BTFSS PORTASTATUS, RA0
	RETLW 00h
	RETLW 01h

RETURN

MC_Iterate
;iteracija petlje - postavlja CURRENT u PREVIOUS
;i obnavlja novi CURRENT

	MOVF CURRENT, W		;trenutni stavimo u prethodni
	MOVWF PREVIOUS

	CALL MC_LoadInputToW	;ucitamo novu vrijednost
	MOVWF CURRENT

RETURN

MC_IsStateChanged
;W je 1 ako je doslo do promjene

	MOVF CURRENT, W
	XORWF PREVIOUS, W

	BTFSS STATUS, Z
	RETLW 01h
	RETLW 00h

RETURN

END