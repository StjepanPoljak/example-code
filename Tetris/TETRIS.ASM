LIST p=PIC16F876
;#include "p16F876.inc"

;Predlozak za PIC16F876 kontroler

;Konfiguracija
;Opcija				Vrij.	Objasnjenje
;-----------------------------------------------------------------
;Code protection		11	(OFF)
;In-Circuit Debugger Mode	1	(OFF)
;Unimplemented			1
;FLASH Write Enable		1	(ON)
;Data EE Memory Protection	1	(OFF)
;Low voltage programming	0	(OFF)
;BOD Reset			1	(ON)
;Code protection		11	(OFF)
;Power-up Timer			0	(ON)
;Watchdog Timer			0	(OFF)
;Oscillator			01	(XT oscillator - crystal)

__CONFIG B'11111101110001'

;Deklaracije registara
;-----------------------------------------------------------------

INDF		EQU	00h
W 		EQU	00h	;working register
F		EQU	01h	;file register
PCL		EQU	02h	;program counter low bits
STATUS		EQU	03h
C		EQU	00h	;8bit carry
Z		EQU	02h	;zero result
FSR		EQU	04h	;Indirect Data Memory Address Pointer
RP0		EQU	05h	;bank select register
PORTA		EQU	05h
PORTB		EQU	06h
PORTC		EQU	07h
PCLATH		EQU	0Ah	;program counter high bits
TMR1L		EQU	0Eh
TMR1H		EQU	0Fh
T1CON		EQU	10h
ADCON0		EQU	1Fh	;AD converter register
ADCON1		EQU	9Fh
TRISA		EQU	85h
TRISB		EQU	86h
TRISC		EQU	87h

;Deklaracije pinova
;-----------------------------------------------------------------

RA0		EQU	00h
RA1		EQU	01h
RA2		EQU	02h
RA3		EQU	03h
RA4		EQU	04h
RA5		EQU	05h

RB0		EQU	00h
RB1		EQU	01h
RB2		EQU	02h
RB3		EQU	03h
RB4		EQU	04h
RB5		EQU	05h
RB6		EQU	06h
RB7		EQU	07h

RC0		EQU	00h
RC1		EQU	01h
RC2		EQU	02h
RC3		EQU	03h
RC4		EQU	04h
RC5		EQU	05h
RC6		EQU	06h
RC7		EQU	07h

;Deklaracije varijabli
;-----------------------------------------------------------------

LO		EQU	20h
HI		EQU	21h
iDelay		EQU	22h
iDelayTemp	EQU	23h
PORTASTATUS	EQU	24h
PORTBSTATUS	EQU	25h
PORTCSTATUS	EQU	26h

RS		EQU	00h
RW		EQU	01h
CE		EQU	02h
RST		EQU	03h
BF		EQU	07h	;busy flag

LCDWaitDelay	EQU	27h
LCDVA		EQU	28h
LCDHA		EQU	29h

RS_OPTION	EQU	2Ah
WRITEBUFFER	EQU	2Bh

;Tetris deklaracije (30h-6Fh)
;-----------------------------------------------------------------

LINE011		EQU	30h
LINE012		EQU	31h
LINE021		EQU	32h
LINE022		EQU	33h
LINE031		EQU	34h
LINE032		EQU	35h
LINE041		EQU	36h
LINE042		EQU	37h
LINE051		EQU	38h
LINE052		EQU	39h
LINE061		EQU	3Ah
LINE062		EQU	3Bh
LINE071		EQU	3Ch
LINE072		EQU	3Dh
LINE081		EQU	3Eh
LINE082		EQU	3Fh
LINE091		EQU	40h
LINE092		EQU	41h
LINE101		EQU	42h
LINE102		EQU	43h
LINE111		EQU	44h
LINE112		EQU	45h
LINE121		EQU	46h
LINE122		EQU	47h
LINE131		EQU	48h
LINE132		EQU	49h
LINE141		EQU	4Ah
LINE142		EQU	4Bh
LINE151		EQU	4Ch
LINE152		EQU	4Dh
LINE161		EQU	4Eh
LINE162		EQU	4Fh
LINE171		EQU	50h
LINE172		EQU	51h
LINE181		EQU	52h
LINE182		EQU	53h
LINE191		EQU	54h
LINE192		EQU	55h
LINE201		EQU	56h
LINE202		EQU	57h

LINE211		EQU	58h
LINE212		EQU	59h
LINE221		EQU	5Ah
LINE222		EQU	5Bh
LINE231		EQU	5Ch
LINE232		EQU	5Dh
LINE241		EQU	5Eh
LINE242		EQU	5Fh

TETVA		EQU	7Dh
TETHA		EQU	7Eh
TETPT		EQU	2Fh

CURR_PT1	EQU	2Ch
BIT_COUNT	EQU	2Dh
COUNT		EQU	2Eh

TEMP_PT		EQU	60h
TEMP_HA		EQU	61h
TEMP_PT2	EQU	62h
TETLOTEMP	EQU	63h
FOURTIME	EQU	64h
TEMP_VA		EQU	65h

PLAYERLINE01	EQU	66h
PLAYERLINE02	EQU	67h
PLAYERLINE03	EQU	68h
PLAYERLINE04	EQU	69h
PLAYERLINE11	EQU	6Ah
PLAYERLINE12	EQU	6Bh
PLAYERLINE13	EQU	6Ch
PLAYERLINE14	EQU	6Dh

PLAYERPIECE	EQU	6Eh
PLAYERROTATION	EQU	6Fh

RANDOM_COUNTER	EQU	70h
XPOSTEMP	EQU	71h
PLAYERXPOS	EQU	72h
PLAYERYPOS	EQU	73h

BUFFPLAYLINE01	EQU	74h
BUFFPLAYLINE02	EQU	75h
BUFFPLAYLINE03	EQU	76h
BUFFPLAYLINE04	EQU	77h
BUFFPLAYLINE11	EQU	78h
BUFFPLAYLINE12	EQU	79h
BUFFPLAYLINE13	EQU	7Ah
BUFFPLAYLINE14	EQU	7Bh

ROTDIR		EQU	7Ch
UNDOPOSITION	EQU	7Fh

;Inicijalizacija programa
;-----------------------------------------------------------------

ORG 00h		;pocetak programa

;Iskljucujemo analogni I/O za port A
BCF ADCON0, 0h

;Prelazimo na bank 1
BSF STATUS, RP0

MOVLW 00h
MOVWF TRISC

MOVLW B'00011111'
MOVWF TRISB

MOVLW 00h
MOVWF TRISA

;Prelazimo na bank 0
BCF STATUS, RP0

CLRF PORTA
CLRF PORTB
CLRF PORTC
CLRF PORTASTATUS

MOVLW B'00110101'
MOVWF T1CON

CLRF TMR1H
CLRF TMR1L

;Glavna petlja
;-----------------------------------------------------------------

CALL LCD_Setup

LStartOver

	CALL LCD_Clear

	CALL Tetris_LoadTitle

	CALL LCD_DrawTable

LResetIntroRandomCounter

	MOVLW 08h
	MOVWF RANDOM_COUNTER

IntroWait

	DECFSZ RANDOM_COUNTER, F
	GOTO LIntroRandomCounterContinue
	GOTO LResetIntroRandomCounter

LIntroRandomCounterContinue

	CALL MC_PORTBDownload

	BTFSS PORTBSTATUS, 04h
	GOTO IntroWait

	CALL MC_PORTBDownload

	BCF PORTBSTATUS, 04h
	MOVLW B'00000101'
	XORWF PORTBSTATUS, W

	BTFSS STATUS, Z
	GOTO IntroWait

CALL Tetris_ClearTable

CALL LCD_DrawTable

CALL New_Piece

MOVLW 03h
CALL Rotate_X_Multiple

CALL Copy_State

MOVLW 0xFF
CALL Draw_Erase_State

CLRF TMR1H
CLRF TMR1L

LRandomCounterReset

	MOVLW 08h
	MOVWF RANDOM_COUNTER

	GOTO LMain

LWePressedDown

	CALL Fall_Down
	CLRF TMR1H
	CLRF TMR1L
	GOTO LMain

LUpdateTetris

	GOTO LMain

LMainFallDown

	CALL Fall_Down

LMain

	DECFSZ RANDOM_COUNTER, F
	GOTO LRandomCounterContinue
	GOTO LRandomCounterReset

LRandomCounterContinue

	MOVLW 0xEE
	MOVWF FOURTIME
	MOVF TMR1H, W
	SUBWF FOURTIME, W

	BTFSS STATUS, C
	GOTO LMainFallDown

	CALL MC_PORTBDownload

	BTFSS PORTBSTATUS, 04h
	GOTO LMain

	CALL MC_PORTBDownload

	BCF PORTBSTATUS, 04h

	MOVLW 02h
	XORWF PORTBSTATUS, W
	BTFSC STATUS, Z
	GOTO LWePressedDown

	MOVLW 03h
	XORWF PORTBSTATUS, W
	BTFSC STATUS, Z
	CALL LeftRightButtonPress

	MOVLW 04h
	XORWF PORTBSTATUS, W
	BTFSC STATUS, Z
	CALL LeftRightButtonPress

	MOVLW 05h
	XORWF PORTBSTATUS, W
	BTFSC STATUS, Z
	CALL Rotate_General_Block

	GOTO LMain

;Microcontroller delay funkcije
;-----------------------------------------------------------------

MC_SDelay
;generira kratki delay, trajanje ucitavamo u LO i LCDWaitDelay

	MOVF LO, W		;1 ciklus
	MOVWF iDelay		;1 ciklus

LSDelay1
	MOVF LCDWaitDelay, W
	MOVWF iDelayTemp	;1 ciklus
LSDelay2
	DECFSZ iDelayTemp, F	;1 ciklus, ali 2 ako je iDelayTemp=0!!!
	GOTO LSDelay2		;2 ciklusa
	DECFSZ iDelay, F	;1 ciklus, ali 2 ako je iDelay=0!!!
	GOTO LSDelay1		;2 ciklusa

RETURN

MC_LDelay
;generira duzi delay, trajanje ucitavamo u LO, HI i LCDWaitDelay

LLDelay
	CALL MC_SDelay		;(3*iDelayTemp+4)*iDelay+3 ciklusa
	DECFSZ HI, F		;1 ciklus, ali 2 ako je HI=0!!!
	GOTO LLDelay		;2 ciklusa
RETURN

;Microcontroller PORT funkcije
;-----------------------------------------------------------------

MC_PORTAUpload
;prebacujemo iz PORTASTATUS u PORTA

	MOVF PORTASTATUS, W
	MOVWF PORTA
	NOP

RETURN

MC_PORTBDownload

	MOVF PORTB, W
	MOVWF PORTBSTATUS
	NOP

RETURN


;LCD funkcije
;-----------------------------------------------------------------

LCD_Setup

	MOVLW D'216'
	MOVWF LO
	MOVLW D'6'
	MOVWF HI
	MOVLW D'255'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo sekundu

	BSF PORTASTATUS, RST	;iskljucujemo reset!
	CALL MC_PORTAUpload

	MOVLW 00h		;PORTC za output
	MOVWF TRISC

	BCF PORTASTATUS, RS	;RS na 0
	CALL MC_PORTAUpload

	BCF PORTASTATUS, RW	;RW na 0
	CALL MC_PORTAUpload

	BCF PORTASTATUS, CE	;CE na 0
	CALL MC_PORTAUpload

	MOVLW D'60'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'255'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 40ms

	MOVLW 00h
	MOVWF RS_OPTION
	MOVLW 30h		;Function - 8bit, basic instruction set
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'35'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 100us

	MOVLW 30h		;Function - 8bit, basic instruction set - drugi put!
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'15'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 40us
	
	MOVLW 0x0C		;Display on, cursor off, blink off
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'35'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 100us

	MOVLW 0x01		;Display clear
	CALL LCD_Write

	MOVLW D'20'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'255'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 10ms

	MOVLW 0x06		;AC increase?
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'25'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 70ms
	
	MOVLW 0x34		;Extended function set
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'25'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 70ms
	
	MOVLW 0x36		;Graphic display ON
	CALL LCD_Write

	MOVLW D'1'
	MOVWF LO
	MOVLW D'1'
	MOVWF HI
	MOVLW D'25'
	MOVWF LCDWaitDelay

	CALL MC_LDelay		;Cekamo cca 70ms			

RETURN

LCD_Read
;Vraca u PORTCSTATUS ono sto procita, u RS_OPTION stavljamo vrijednost registra (00h - function, 01h - data)

	MOVLW 0xFF		;postavljamo PORTC na input
	MOVWF TRISC

	BCF PORTASTATUS, RS	;postavimo RS na 0
	BTFSC RS_OPTION, 00h	;je li nam least significant bit od RS_OPTION 0?
	BSF PORTASTATUS, RS	;ako nije, postavimo RS na 1
	CALL MC_PORTAUpload

	NOP

	BSF PORTASTATUS, RW	;postavljamo RW na 1 - za citanje
	CALL MC_PORTAUpload

	NOP

	BSF PORTASTATUS, CE	;postavljamo CE na 1
	CALL MC_PORTAUpload

	NOP

	MOVF PORTC, W		;prebacujemo podatke u PORTCSTATUS
	MOVWF PORTCSTATUS

	BCF PORTASTATUS, CE	;postavljamo CE na 0
	CALL MC_PORTAUpload

RETURN

LCD_Write
;RS_OPTION - biranje registra (ako je DATA, tj. 1, onda u HI stavljamo drugi dio podatka), W - podatak za slanje

	MOVWF WRITEBUFFER	;prebacujemo podatke u WRITEBUFFER

	;MOVLW 00h
	;MOVWF RS_OPTION
	BCF PORTASTATUS, RS	;postavljamo RS na 0 - function
	CALL MC_PORTAUpload

LCD_Wait
	CALL LCD_Read		;citamo podatke sa PORTC
	BTFSS PORTCSTATUS, BF	;je li busy flag 0?
	GOTO LCD_Wait		;ako nije - ponavljamo, ako je - preskacemo

	MOVLW 00h		;postavljamo PORTC na output
	MOVWF TRISC

	BCF PORTASTATUS, RS	;postavimo RS na 0
	BTFSC RS_OPTION, 00h	;je li nam least significant bit od RS_OPTION 0?
	BSF PORTASTATUS, RS	;ako nije, postavimo RS na 1
	CALL MC_PORTAUpload

	NOP

	BCF PORTASTATUS, RW	;postavimo RW na 0 - write
	CALL MC_PORTAUpload

	NOP

	BCF PORTASTATUS, CE	;postavimo CE na 0
	CALL MC_PORTAUpload

	MOVF WRITEBUFFER, W	;prebacimo iz buffera u W
	MOVWF PORTC		;i uploadamo na PORTC

	NOP

	BSF PORTASTATUS, CE	;postavimo CE na 1
	CALL MC_PORTAUpload

	NOP
	NOP
	NOP
	NOP
	NOP

	BCF PORTASTATUS, CE	;postavimo CE na 0
	CALL MC_PORTAUpload	

	BTFSS RS_OPTION, 00h	;saljemo li podatke, tj. je li RS=1?
	RETURN			;RS je nula, ne saljemo podatke i gotovi smo

	NOP			;saljemo podatke, pa cemo poslati i high byte

	MOVF HI, W		;stavljamo podatke iz registra HI na PORTC
	MOVWF PORTC

	NOP

	BSF PORTASTATUS, CE	;postavimo chip enable
	CALL MC_PORTAUpload

	NOP
	NOP
	NOP
	NOP
	NOP

	BCF PORTASTATUS, CE	;iskljucimo chip enable
	CALL MC_PORTAUpload

	NOP			;podatci su poslani, izlazimo...

RETURN

LCD_Clear

	MOVLW 00h		;postavljamo vertikalnu adresu na 00h
	MOVWF LCDVA

Vertical			;pocetak petlje za vertikalnu adresu

	MOVLW 00h		;postavljamo horizontalnu adresu na 00h
	MOVWF LCDHA

Horizontal			;pocetak petlje za horizontalnu adresu

	BCF RS_OPTION, 00h	;postavljamo RS na 0, tj. function
	MOVLW 0x80		;stavljamo b'10000000' u W
	IORWF LCDVA, W		;vrsimo OR operaciju sa vertikalnom adresom i W registrom
	CALL LCD_Write		;dobili smo b'10VVVVVV' i saljemo to LCD-u

	BCF RS_OPTION, 00h	;postavljamo RS na 0 opet...
	MOVLW 0x80		;stavljamo b'10000000' u W
	IORWF LCDHA, W		;vrsimo OR operaciju, ali sa horizontalnom adresom
	CALL LCD_Write		;dobili smo b'1000HHHH' i saljemo to LCD-u

	BSF RS_OPTION, 00h	;postavljamo RS na 1 - data
	MOVLW 00h		;postavljamo high podatke na 00h
	MOVWF HI
	MOVLW 00h		;postavljamo low podatke na 00h
	CALL LCD_Write		;saljemo to LCD-u

	INCF LCDHA, F		;povecavamo brojac za horizontalnu adresu

	MOVLW B'00010000'	;ako smo presli b'00001111'...
	XORWF LCDHA, W		;je li horizontalna adresa sad b'00010000'?

	BTFSS STATUS, Z

	GOTO Horizontal		;ako nije, onda ponavljamo petlju

	INCF LCDVA, F		;ako je, povecavamo brojac vertikalne adrese

	MOVLW B'01000000'	;gledamo je li vertikalni brojac presao b'00111111'
	XORWF LCDVA, W		;je li sad b'01000000'?

	BTFSS STATUS, Z

	GOTO Vertical		;ako nije, onda ponavljamo petlju

	BCF PORTASTATUS, RS	;ako je, onda postavimo RS na 0
	CALL MC_PORTAUpload	;i gotovi smo

	NOP

RETURN

LCD_DrawTable

	MOVLW 00h
	MOVWF TEMP_HA
	MOVLW 00h
	MOVWF TEMP_PT

	MOVLW D'12'		;postavimo vertikalnu adresu na 11
;	MOVLW D'0'
	MOVWF LCDVA

	BCF TETPT, 00h
	BCF RS_OPTION, 01h

	MOVLW HIGH Tetris_Table
	MOVWF PCLATH		;ucitavamo adresu od Tetris_Table u PCLATH

	MOVLW 07h
	MOVWF TETVA

TetrisVertical

	MOVLW D'1'		;postavimo horizontalnu adresu na 5
	BTFSC RS_OPTION, 01h
	MOVLW D'9'
	MOVWF LCDHA

	MOVLW D'1'
	MOVWF TETHA

TetrisHorizontal

	MOVF TETHA, W		;horizontalnu adresu prebacimo u count...
	CALL Tetris_Table	;trazimo liniju sa vrijednosti iz COUNT i vracamo u W
	MOVWF CURR_PT1

	BTFSS TETPT, 0h		;jesmo li u prvom dijelu? ako jesmo, onda preskacemo iducu liniju
	GOTO SkipTable21

	MOVF TETHA, W
	CALL Tetris_Table2	;nismo preskocili ovu liniju, znaci na drugom smo dijelu, pa prepisemo W sa				;vrijednosti iz Tetris_Table2
	MOVWF CURR_PT1		;provjeravamo imamo li tu kockicu...

SkipTable21

	MOVF TETVA, W
	CALL Tetris_BitTable	;samo iscitamo vrijednost iz tablice...
	MOVWF TEMP_PT
	
	INCF TETHA, F		;povecamo brojac TETHA za 1
	MOVF TETHA, W
	CALL Tetris_Table	;iscitamo vrijednost iz tablice
	MOVWF CURR_PT1

	BTFSS TETPT, 0h		;ako smo na drugom dijelu, citamo vrijednost iz tablice 2
	GOTO SkipTable22

	MOVF TETHA, W
	CALL Tetris_Table2
	MOVWF CURR_PT1

SkipTable22

	MOVF TETVA, W
	CALL Tetris_BitTable
	MOVWF TEMP_PT2
	BTFSS TEMP_PT2, 07h	;ako MSB=1 znaci da imamo kockicu i trebamo ju dodati u TEMP_PT sa OR
	GOTO SkipIOR		;ako MSB=0 nemamo kockicu i preskacemo ovo...

	COMF TEMP_PT2
	MOVF TEMP_PT2, W
	IORWF TEMP_PT, F

SkipIOR

	INCF TETHA, F

	BTFSC TEMP_HA, 00h	;gledamo TEMP_HA brojac, ako je paran, preskacemo
	GOTO SendData		;ako je neparan saljemo podatke

	MOVF TEMP_PT, W
	MOVWF TETLOTEMP

	GOTO SkipSendData	

SendData

	CLRF FOURTIME
	MOVF LCDVA, W
	MOVWF TEMP_VA

FourTimeLoop

	BCF RS_OPTION, 00h	;postavljamo RS na 0, tj. function
	MOVLW 0x80		;stavljamo b'10000000' u W
	IORWF LCDVA, W		;vrsimo OR operaciju sa vertikalnom adresom i W registrom
	CALL LCD_Write		;dobili smo b'10VVVVVV' i saljemo to LCD-u

	BCF RS_OPTION, 00h	;postavljamo RS na 0 opet...
	MOVLW 0x80		;stavljamo b'10000000' u W
	IORWF LCDHA, W		;vrsimo OR operaciju, ali sa horizontalnom adresom
	CALL LCD_Write		;dobili smo b'1000HHHH' i saljemo to LCD-u

	BSF RS_OPTION, 00h	;postavljamo RS na 1 - data

	MOVF TEMP_PT, W
	MOVWF HI
	MOVF TETLOTEMP, W
	CALL LCD_Write		;saljemo to LCD-u

	INCF LCDVA, F
	INCF FOURTIME, F

	MOVLW D'4'
	XORWF FOURTIME, W

	BTFSS STATUS, Z
	GOTO FourTimeLoop

	MOVF TEMP_VA, W
	MOVWF LCDVA

	INCF LCDHA, F

SkipSendData

	INCF TEMP_HA, F
	
	MOVLW D'21'		;je li TETHA 21?
	XORWF TETHA, W

	BTFSS STATUS, Z		;je li TETHA=21?

	GOTO TetrisHorizontal	;ako nije, nastavljamo... ako je, onda skacemo nize vertikalno

	MOVLW D'4'
	ADDWF LCDVA, F

	DECF TETVA, F		;povecavamo adresu u tablici za bitove (retke)

	MOVLW D'2'
	XORWF TETVA, W
	
	BTFSS STATUS, Z
	GOTO SkipReset

	MOVLW D'0'		;postavljamo vertikalnu adresu na 0 (ovo je neki bug ili nesto)
	MOVWF LCDVA

	BSF RS_OPTION, 01h

SkipReset

	BTFSC TETPT, 0h		;testiramo LSB od TETPT
	GOTO TestPart2		;ako je TETPT=1 idemo na Part2

	MOVLW D'255'		;ako smo presli broj 7, tj. sad smo na 8...
	XORWF TETVA, W		;ako je TETVA=8 => Z=1

	BTFSS STATUS, Z		;je li Z=1?
	GOTO TetrisVertical	;Z=0, pa nastavljamo...
	
	BSF TETPT, 0h		;TETVA=8, pa prelazimo na TETVA=0 i TETPT
	MOVLW D'7'		;postavimo TETVA=0 (to nam u biti predstavlja bitove samo...)
	MOVWF TETVA

	GOTO TetrisVertical	;idemo natrag u petlju

TestPart2

	MOVLW D'5'		;ako smo presli broj 1, tj. sad smo na 2...
	XORWF TETVA, W		;ako je TETVA=2 => Z=1

	BTFSS STATUS, Z		;je li Z=1?
	GOTO TetrisVertical	;Z=0, pa nastavljamo...

RETURN				;ako je TETVA=2, gotovi smo skroz

New_Piece

	MOVLW d'20'		;postavljamo Y poziciju igraca na 20
	MOVWF PLAYERYPOS

	MOVLW 01h		;postavljamo X poziciju igraca na 1
	MOVWF PLAYERXPOS

	MOVLW 01h		;postavljamo rotaciju na 1
	MOVWF PLAYERROTATION

	CLRF BUFFPLAYLINE11	;postavljamo BUFFPLAYLINE1X na 0
	CLRF BUFFPLAYLINE12	;(ovo su ona dva bita koji su na
	CLRF BUFFPLAYLINE13	;pocetku sigurno prazni)
	CLRF BUFFPLAYLINE14

	MOVLW 01h		;ako je RANDOM_COUNTER=01
	XORWF RANDOM_COUNTER, W	;dodajemo kocku
	BTFSS STATUS, Z		;itd...
	GOTO LNext2
	CALL Make_Square
	RETURN
LNext2
	MOVLW 02h
	XORWF RANDOM_COUNTER, W
	BTFSS STATUS, Z
	GOTO LNext3
	CALL Make_Line1
	RETURN

LNext3
	MOVLW 03h
	XORWF RANDOM_COUNTER, W
	BTFSS STATUS, Z
	GOTO LNext4
	CALL Make_TBlock1
	RETURN

LNext4
	MOVLW 04h
	XORWF RANDOM_COUNTER, W
	BTFSS STATUS, Z
	GOTO LNext5
	CALL Make_LLBlock1
	RETURN

LNext5
	MOVLW 05h
	XORWF RANDOM_COUNTER, W
	BTFSS STATUS, Z
	GOTO LNext6
	CALL Make_RLBlock1
	RETURN

LNext6
	MOVLW 06h
	XORWF RANDOM_COUNTER, W
	BTFSS STATUS, Z
	GOTO LNext7
	CALL Make_LSquiggly1
	RETURN
LNext7
	CALL Make_RSquiggly1
	RETURN

Copy_State
;Kopira iz BUFFPLAYLINE u PLAYERLINE

	MOVLW 08h		;postavljamo petlju na 8
	MOVWF FOURTIME		;jer kopiramo svih 8 linija

LCopyLoop

	MOVLW 73h		;citamo podatke iz 73h+FOURTIME
	ADDWF FOURTIME, W
	
	MOVWF FSR
	MOVF INDF, W		;iscitamo to iz INDF
	MOVWF LO		;i prebacimo u LO

	MOVLW 65h		;ucitavamo podatke u 65h+FOURTIME
	ADDWF FOURTIME, W

	MOVWF FSR		;spremamo iz LO u INDF
	MOVF LO, W
	MOVWF INDF

	DECFSZ FOURTIME, F	;nova iteracija petlje
	GOTO LCopyLoop

	RETURN

Undo_State
;Radi isto sto i Copy_State samo obratno:
;kopira iz PLAYERLINE u BUFFPLAYLINE

	MOVLW 08h
	MOVWF FOURTIME

LUndoStateLoop

	MOVLW 65h
	ADDWF FOURTIME, W
	
	MOVWF FSR
	MOVF INDF, W
	MOVWF LO

	MOVLW 73h
	ADDWF FOURTIME, W

	MOVWF FSR
	MOVF LO, W
	MOVWF INDF

	DECFSZ FOURTIME, F
	GOTO LUndoStateLoop

	RETURN

Check_State
;vraca W=00h ako se ne moze nacrtati, FFh ako moze

	MOVLW 04h		;ponovit cemo petlju jedanput za svaki red
	MOVWF FOURTIME

LCheckStateLoop

	MOVLW 73h		;da bi dobili indirektnu adresu reda
	ADDWF FOURTIME, W	;zbrajamo adresu 73h s brojacem FOURTIME

	MOVWF FSR		;spremamo to u FSR

	MOVF INDF, W		;citamo vrijednost iz registra INDF
	MOVWF HI		;i spremamo u HI

	MOVF FOURTIME, W	;ucitat cemo redak iz tablice...
	MOVWF LO		;ali prvo racunamo koja je to linija
	MOVF PLAYERYPOS, W	;u tablici, a to dobivamo kao
	ADDWF LO, F		;FOURTIME+PLAYERYPOS-1
	DECF LO, F

	MOVLW HIGH Tetris_Table
	MOVWF PCLATH
	MOVF LO, W		;iscitavamo taj redak iz tablice
	CALL Tetris_Table

	ANDWF HI, W		;pravimo AND s tim retkom
	BTFSS STATUS, Z		;ako je rezultat operacije nula,
	RETLW 00h		;znaci da se nista nije poklopilo, pa je polje slobodno
				;(1100)AND(0011)=0 => Z=1, (1110)AND(0011)!=0 => Z=0

	MOVLW 77h		;isto radimo jos jednom, samo za drugi dio
	ADDWF FOURTIME, W	;linije igraca

	MOVWF FSR

	MOVF INDF, W
	MOVWF HI		;i spremamo u HI

	MOVF FOURTIME, W	;ucitat cemo redak iz tablice...
	MOVWF LO
	MOVF PLAYERYPOS, W
	ADDWF LO, F
	DECF LO, F

	MOVLW HIGH Tetris_Table2
	MOVWF PCLATH
	MOVF LO, W
	CALL Tetris_Table2

	ANDWF HI, W
	BTFSS STATUS, Z
	RETLW 00h

	DECFSZ FOURTIME, F
	GOTO LCheckStateLoop

	RETLW 0xFF

;ORG 0x200

Draw_Erase_State
;W=00h za Erase, W=FFh za Draw

	MOVWF ROTDIR
	MOVLW 04h		;ponovit cemo petlju jedanput za svaki red
	MOVWF FOURTIME

LDrawStateLoop

	MOVF FOURTIME, W	;ucitat cemo redak iz tablice...
	MOVWF LO
	MOVF PLAYERYPOS, W
	ADDWF LO, F
	DECF LO, F

	MOVLW HIGH Tetris_Table
	MOVWF PCLATH
	MOVF LO, W
	CALL Tetris_Table
	MOVWF HI

	MOVLW 65h		;da bi dobili indirektnu adresu reda
	ADDWF FOURTIME, W	;zbrajamo adresu 65h s brojacem FOURTIME

	MOVWF FSR		;spremamo to u FSR

	MOVF INDF, W		;citamo vrijednost iz registra INDF
	BTFSS ROTDIR, 00h
	GOTO LErase1
	GOTO LDraw1

LErase1
	XORWF HI, W		;ako zelimo obrisati ono sto se nalazi
	GOTO LSkip1		;na tom polju, koristimo XOR
				;(001100)XOR(101110)=100010
LDraw1
	IORWF HI, W		;ako zelimo dodati red u polje koristimo OR
				;(001100)IOR(100010)=101110
LSkip1
	MOVWF CURR_PT1		;spremamo rezultat u CURR_PT1

	MOVLW HIGH Tetris_TableModify
	MOVWF PCLATH		;ucitavamo adresu od Tetris_Table u PCLATH

	MOVF LO, W		;ucrtavamo to u tablicu
	CALL Tetris_TableModify

	MOVLW HIGH Tetris_Table2
	MOVWF PCLATH
	MOVF LO, W
	CALL Tetris_Table2
	MOVWF HI

	MOVLW 69h		;isto radimo jos jednom, samo za drugi dio
	ADDWF FOURTIME, W	;linije igraca

	MOVWF FSR

	MOVF INDF, W
	BTFSS ROTDIR, 00h
	GOTO LErase2
	GOTO LDraw2
LErase2
	XORWF HI, W
	GOTO LSkip2
LDraw2
	IORWF HI, W
LSkip2
	MOVWF CURR_PT1

	MOVLW HIGH Tetris_TableModify2
	MOVWF PCLATH		;ucitavamo adresu od Tetris_Table u PCLATH

	MOVF LO, W
	CALL Tetris_TableModify2

	DECFSZ FOURTIME, F
	GOTO LDrawStateLoop	

	BTFSC ROTDIR, 00h
	CALL LCD_DrawTable

	RETURN

Rotate_X
;ROTDIR je smjer, 00h za desno, FFh za lijevo
;vraca W=00h ako je bio neuspjeh, FFh za uspjeh

	MOVLW 04h		;ponovit cemo petlju jedanput za svaki red
	MOVWF FOURTIME

LRotateXLoop

	MOVLW 73h		;da bi dobili indirektnu adresu reda
	ADDWF FOURTIME, W	;zbrajamo adresu 73h s brojacem FOURTIME

	MOVWF FSR		;spremamo to u FSR

	MOVF INDF, W		;citamo vrijednost iz registra INDF
	MOVWF LO		;i spremamo u LO

	MOVLW 77h		;isto radimo jos jednom, samo za drugi dio
	ADDWF FOURTIME, W	;linije igraca

	MOVWF FSR

	MOVF INDF, W
	MOVWF HI		;i spremamo u HI

	BTFSC ROTDIR, 00h
	GOTO LRotateLeft
	GOTO LRotateRight

LRotateLeft

	BTFSC LO, 07h		;ako pomicemo liniju u lijevo...
	RETLW 00h		;provjeravamo nalazi li se na MSB od LO jedinica
				;tj. ako je 10000000 00 - ne mozemo se pomicati
				;u lijevo, pa smo gotovi
	RLF LO, F		;ako mozemo, pomicemo polje u lijevo
	BCF LO, 00h		;postavljamo LSB od LO na 0 (jer RLF zna dodati 1)

	BTFSC HI, 07h		;ako smo na MSB od HI imali 1, tj.
	BSF LO, 00h		;npr. 00000000 10, postavljamo LSB od LO na 1

	RLF HI, F		;pomicemo i HI u lijevo
	BCF HI, 00h		;i postavljamo LSB od HI na 0

	GOTO LRotateEnd

LRotateRight

	BTFSC HI, 06h		;ako pomicemo liniju u desno...
	RETLW 00h		;provjeravamo nalazi li se na 06h od HI jedinica
				;tj. ako je 00000000 01 - ne mozemo se pomicati
				;u desno, pa smo gotovi
	RRF HI, F		;pomicemo HI u desno
	BCF HI, 07h		;postavljamo MSB od HI na 0 (RRF zna dodati 1)

	BTFSC LO, 00h		;ako smo na LSB od LO imali 1, tj.
	BSF HI, 07h		;npr. 00000001 00, postavljamo MSB od HI na 1

	RRF LO, F		;pomicemo i LO u desno
	BCF LO, 07h		;i postavljamo MSB od LO na 0

LRotateEnd

	MOVLW 73h		;sada to moramo spremiti natrag,
	ADDWF FOURTIME, W	;pa kopiramo sadrzaj iz LO i HI
				;natrag u BUFFPLAYLINE
	MOVWF FSR

	MOVF LO, W
	MOVWF INDF

	MOVLW 77h
	ADDWF FOURTIME, W

	MOVWF FSR

	MOVF HI, W
	MOVWF INDF

	DECFSZ FOURTIME, F
	GOTO LRotateXLoop

	BTFSS ROTDIR, 00h
	GOTO LWeWereRight
	DECF PLAYERXPOS, F	;smanjujemo X poziciju

	RETLW 0xFF

LWeWereRight

	INCF PLAYERXPOS, F	;povecavamo X poziciju

	RETLW 0xFF

Make_Square

	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVWF BUFFPLAYLINE01

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW 01h
	MOVWF PLAYERPIECE

	RETURN

Make_Line1

	MOVLW B'11110000'
	MOVWF BUFFPLAYLINE01

	CLRF BUFFPLAYLINE02
	CLRF BUFFPLAYLINE03
	CLRF BUFFPLAYLINE04

	MOVLW 02h
	MOVWF PLAYERPIECE

	RETURN

Make_Line2

	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE01
	MOVWF BUFFPLAYLINE02
	MOVWF BUFFPLAYLINE03
	MOVWF BUFFPLAYLINE04

	RETURN

Make_TBlock1

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE01

	MOVLW 03h
	MOVWF PLAYERPIECE

	RETURN

Make_TBlock2

	CLRF BUFFPLAYLINE04
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE01
	MOVWF BUFFPLAYLINE03

	RETURN

Make_TBlock3

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_TBlock4

	CLRF BUFFPLAYLINE04
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE01
	MOVWF BUFFPLAYLINE03

	RETURN

Make_RSquiggly1

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03
	MOVLW B'01100000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE01

	MOVLW 04h
	MOVWF PLAYERPIECE

	RETURN

Make_RSquiggly2

	CLRF BUFFPLAYLINE04
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_LSquiggly1

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'01100000'
	MOVWF BUFFPLAYLINE01

	MOVLW 05h
	MOVWF PLAYERPIECE

	RETURN

Make_LSquiggly2

	CLRF BUFFPLAYLINE04
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_RLBlock1

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'00100000'
	MOVWF BUFFPLAYLINE01

	MOVLW 06h
	MOVWF PLAYERPIECE

	RETURN

Make_RLBlock2

	CLRF BUFFPLAYLINE04
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_RLBlock3

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_RLBlock4

	MOVLW B'00000000'
	MOVWF BUFFPLAYLINE04
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_LLBlock1

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE01

	MOVLW 07h
	MOVWF PLAYERPIECE

	RETURN

Make_LLBlock2

	CLRF BUFFPLAYLINE04
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_LLBlock3

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'00100000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE01

	RETURN

Make_LLBlock4

	CLRF BUFFPLAYLINE04
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE03
	MOVLW B'10000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11000000'
	MOVWF BUFFPLAYLINE01

	RETURN

;ORG 0x300

LeftRightButtonPress

	CALL Undo_State		;prvo cemo, sigurnosti radi kopirati
				;PLAYERLINE u BUFFPLAYLINE
	MOVLW 0xFF		;pomicemo u lijevo
	MOVWF ROTDIR

	MOVLW 03h		;ako je pritisnuta lijeva tipka
	XORWF PORTBSTATUS, W
	BTFSS STATUS, Z		;preskacemo...
	CLRF ROTDIR		;inace pomicemo u desno

	MOVF PLAYERXPOS, W	;spremamo X poziciju
	MOVWF UNDOPOSITION

	CALL Rotate_X		;pokusavamo rotirati
	MOVWF ROTDIR		;W=FFh ako smo uspjeli rotirati, 00h ako nismo
	BTFSS ROTDIR, 00h	;jesmo li uspjeli rotirati
	RETURN			;nismo, vracamo se
	CALL Animate_Iteration	;jesmo, pokusat cemo to sad kopirati u tablicu
	MOVWF ROTDIR		;jesmo li uspjeli animirati?
	BTFSS ROTDIR, 00h
	GOTO LRotateFail
	RETURN

LRotateFail
	MOVF UNDOPOSITION, W	;vracamo X poziciju, ako nismo uspjeli
	MOVWF PLAYERXPOS

	RETURN

Fall_Down

	CALL Undo_State		;prvo cemo, sigurnosti radi kopirati
				;PLAYERLINE u BUFFPLAYLINE
	MOVF PLAYERYPOS, W	;spremamo X poziciju
	MOVWF UNDOPOSITION

	DECFSZ PLAYERYPOS, F	;smanjimo Y poziciju, odnosno redak za 1
	GOTO LFallNormal
	GOTO LFallFail		;ako ne mozemo vise smanjivati, gotovi smo

LFallNormal

	INCF PLAYERYPOS, F	;vracamo se na prethodnu Y poziciju
	MOVLW 00h
	CALL Draw_Erase_State	;brisemo odgovarajuce blokove iz tablice
	DECF PLAYERYPOS, F	;opet smanjujemo Y poziciju
	CALL Check_State	;provjeravamo mozemo li ugraditi blokove u tablicu
	MOVWF ROTDIR
	BTFSS ROTDIR, 00h
	GOTO LNormalFallFail	;ako ne mozemo, idemo na LNormalFallFail
	CALL Copy_State		;kopiramo iz BUFFPLAYLINE u PLAYERLINE
	MOVLW 0xFF
	CALL Draw_Erase_State	;ucrtavamo blokove u tablicu
	;CALL LCD_DrawTable	;crtamo na LCD

	CLRF TMR1H		;resetiramo timer
	CLRF TMR1L

	RETURN

LNormalFallFail

	INCF PLAYERYPOS, F	;vratimo se na prijasnju poziciju
	MOVLW 0xFF		;i ucrtamo natrag ono sto smo obrisali
	CALL Draw_Erase_State

LFallFail

LOnesCheckingLoop
	CALL CheckIfThereIsARowOfOnes
	MOVWF ROTDIR

	MOVLW 00h
	XORWF ROTDIR, W
	BTFSC STATUS, Z
	GOTO LWeReNotGonnaFall

	MOVF ROTDIR, W
	CALL MakeRowsFallDown
	GOTO LOnesCheckingLoop

LWeReNotGonnaFall

	CALL LCD_DrawTable

	CALL New_Piece		;u svakom slucaju, gotovi smo, pa dodajemo novi blok

	MOVLW 03h		;prebacujemo blok na pocetnu poziciju

	CALL Rotate_X_Multiple

	CALL Check_State	;provjeravamo mozemo li ga ugraditi
	MOVWF ROTDIR
	BTFSS ROTDIR, 00h
	GOTO LStartOver		;ako ne mozemo, igra je gotova

	CALL Copy_State		;kopiramo iz BUFFPLAYLINE u PLAYERLINE

	MOVLW 0xFF
	CALL Draw_Erase_State	;ucrtavamo ga u tablicu

	CLRF TMR1H		;resetiramo timer
	CLRF TMR1L

	RETURN

Animate_Iteration
;obavlja jednu iteraciju animacije
;vraca W=00h ako je neuspjesno kopirano u tablicu, FFh ako smo uspjeli

	MOVLW 00h
	CALL Draw_Erase_State	;brisemo prijasnje stanje (BUFFPLAYLINE)
	CALL Check_State	;provjeravamo mozemo li ubaciti novi blok (PLAYERLINE)
	MOVWF ROTDIR		;ako ne mozemo idemo na LAnimationFail
	BTFSS ROTDIR, 00h
	GOTO LAnimationFail
	CALL Copy_State		;ako mozemo, kopiramo PLAYERLINE u Tetris tablicu
	MOVLW 0xFF
	CALL Draw_Erase_State	;ucrtavamo to...
	CALL LCD_DrawTable	;crtamo na LCD

	RETLW 0xFF

LAnimationFail			;nismo uspjeli animirati

	CALL Undo_State		;vracamo prijasnje stanje iz PLAYERLINE u BUFFPLAYLINE
	MOVLW 0xFF
	CALL Draw_Erase_State	;ucrtavamo ga opet

	RETLW 00h

Rotate_X_Multiple
;W je koliko linija trebamo pomicati

	MOVWF CURR_PT1		;stavljamo broj ponavljanja u CURR_PT1

LXMultipleLoop

	DECFSZ CURR_PT1, F	;jedna iteracija...
	GOTO LContinueNormal
	RETURN

LContinueNormal

	CLRF ROTDIR		;
	CALL Rotate_X
	MOVWF ROTDIR
	BTFSS ROTDIR, 00h
	RETURN

	GOTO LXMultipleLoop

	RETURN

Rotate_General_Block

	CALL Undo_State		;prvo cemo, sigurnosti radi kopirati
				;PLAYERLINE u BUFFPLAYLINE
	CLRF BUFFPLAYLINE11
	CLRF BUFFPLAYLINE12
	CLRF BUFFPLAYLINE13
	CLRF BUFFPLAYLINE14

	MOVF PLAYERROTATION, W
	MOVWF UNDOPOSITION

	INCF PLAYERROTATION, F

	MOVF PLAYERXPOS, W
	MOVWF XPOSTEMP

	MOVLW 01h
	MOVWF PLAYERXPOS

	MOVLW 02h
	XORWF PLAYERPIECE, W	
	BTFSC STATUS, Z
	GOTO LRotateLine

	MOVLW 03h
	XORWF PLAYERPIECE, W	
	BTFSC STATUS, Z
	GOTO LRotateTBlock

	MOVLW 04h
	XORWF PLAYERPIECE, W	
	BTFSC STATUS, Z
	GOTO LRotateRSquiggly

	MOVLW 05h
	XORWF PLAYERPIECE, W	
	BTFSC STATUS, Z
	GOTO LRotateLSquiggly

	MOVLW 06h
	XORWF PLAYERPIECE, W
	BTFSC STATUS, Z
	GOTO LRotateRLBlock

	MOVLW 07h
	XORWF PLAYERPIECE, W
	BTFSC STATUS, Z
	GOTO LRotateLLBlock

	GOTO LRotateBlockFail

LRotateTBlock

	CALL Rotate_TBlock
	GOTO LRotateEndGeneral

LRotateRLBlock

	CALL Rotate_RLBlock
	GOTO LRotateEndGeneral

LRotateLLBlock

	CALL Rotate_LLBlock
	GOTO LRotateEndGeneral

LRotateLine

	CALL Rotate_Line
	GOTO LRotateEndGeneral

LRotateLSquiggly

	CALL Rotate_LSquiggly
	GOTO LRotateEndGeneral

LRotateRSquiggly

	CALL Rotate_RSquiggly
	GOTO LRotateEndGeneral

LRotateEndGeneral

	MOVF XPOSTEMP, W
	CALL Rotate_X_Multiple

	CALL Animate_Iteration
	MOVWF ROTDIR
	BTFSS ROTDIR, 00h
	GOTO LRotateBlockFail
	RETURN

LRotateBlockFail

	MOVF UNDOPOSITION, W	;vracamo X poziciju, ako nismo uspjeli
	MOVWF PLAYERROTATION

	MOVF XPOSTEMP, W
	MOVWF PLAYERXPOS

	CALL Undo_State

	RETURN

Rotate_TBlock

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_TBlock2

	MOVLW 03h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_TBlock3

	MOVLW 04h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_TBlock4

	CALL Make_TBlock1

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_TBlock2

	CALL Make_TBlock2
	RETURN

LMake_TBlock3

	CALL Make_TBlock3
	RETURN

LMake_TBlock4

	CALL Make_TBlock4
	RETURN

Rotate_RLBlock

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_RLBlock2

	MOVLW 03h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_RLBlock3

	MOVLW 04h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_RLBlock4

	CALL Make_RLBlock1

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_RLBlock2

	CALL Make_RLBlock2
	RETURN

LMake_RLBlock3

	CALL Make_RLBlock3
	RETURN

LMake_RLBlock4

	CALL Make_RLBlock4
	RETURN

Rotate_LLBlock

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_LLBlock2

	MOVLW 03h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_LLBlock3

	MOVLW 04h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_LLBlock4

	CALL Make_LLBlock1

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_LLBlock2

	CALL Make_LLBlock2
	RETURN

LMake_LLBlock3

	CALL Make_LLBlock3
	RETURN

LMake_LLBlock4

	CALL Make_LLBlock4
	RETURN

Rotate_LSquiggly

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_LSquiggly2

	CALL Make_LSquiggly1

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_LSquiggly2

	CALL Make_LSquiggly2
	RETURN


Rotate_RSquiggly

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_RSquiggly2

	CALL Make_RSquiggly1

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_RSquiggly2

	CALL Make_RSquiggly2
	RETURN


Rotate_Line

	MOVLW 02h
	XORWF PLAYERROTATION, W
	BTFSC STATUS, Z
	GOTO LMake_Line2

	CALL Make_Line1

;	MOVLW 01h
;	XORWF XPOSTEMP, W
;	BTFSS STATUS, Z
;	DECF XPOSTEMP, F

;	INCF PLAYERYPOS, F

	MOVLW 01h
	MOVWF PLAYERROTATION

	RETURN

LMake_Line2

;	MOVLW 01h
;	XORWF XPOSTEMP, W
;	BTFSC STATUS, Z
;	RETURN

	CALL Make_Line2

;	INCF XPOSTEMP, F
;	DECF PLAYERYPOS, F

	RETURN

CheckIfThereIsARowOfOnes
;vraca W=broj reda ako je neki red jednak jedan skroz, 00h ako nije

	MOVLW d'24'
	MOVWF FOURTIME

LCheckOnesLoop

	MOVLW HIGH Tetris_Table
	MOVWF PCLATH

	MOVF FOURTIME, W
	CALL Tetris_Table
	MOVWF ROTDIR

	MOVLW B'11111111'
	XORWF ROTDIR, W

	BTFSS STATUS, Z
	GOTO LContinueWithCheckOnes

	MOVLW HIGH Tetris_Table2
	MOVWF PCLATH

	MOVF FOURTIME, W
	CALL Tetris_Table2
	MOVWF ROTDIR

	MOVLW B'11000000'
	ANDWF ROTDIR, F

	MOVLW B'11000000'
	XORWF ROTDIR, W

	BTFSS STATUS, Z
	GOTO LContinueWithCheckOnes

	MOVF FOURTIME, W
	RETURN
		
LContinueWithCheckOnes

	DECFSZ FOURTIME, F
	GOTO LCheckOnesLoop

	MOVLW 00h
	RETURN

MakeRowsFallDown
;ako smo ponistili red W=XXh, spustamo sve ostale...

	MOVWF FOURTIME

LRowsFallDownLoop

	INCF FOURTIME, F

	MOVLW HIGH Tetris_Table
	MOVWF PCLATH

	MOVF FOURTIME, W
	CALL Tetris_Table
	MOVWF ROTDIR

	DECF FOURTIME, F

	MOVLW HIGH Tetris_TableModify
	MOVWF PCLATH

	MOVF ROTDIR, W
	MOVWF CURR_PT1
	MOVF FOURTIME, W
	CALL Tetris_TableModify

	INCF FOURTIME, F

	MOVLW HIGH Tetris_Table2
	MOVWF PCLATH

	MOVF FOURTIME, W
	CALL Tetris_Table2
	MOVWF ROTDIR

	DECF FOURTIME, F

	MOVLW HIGH Tetris_TableModify2
	MOVWF PCLATH

	MOVF ROTDIR, W
	MOVWF CURR_PT1
	MOVF FOURTIME, W
	CALL Tetris_TableModify2

	INCF FOURTIME, F

	MOVLW d'24'
	XORWF FOURTIME, W
	BTFSS STATUS, Z
	GOTO LRowsFallDownLoop

	RETURN

ORG 0x0700

;Tablice za iscitavanje vrijednosti redaka

Tetris_Table

	MOVWF COUNT		;koristimo indirektno adresiranje
	MOVLW 01h
	SUBWF COUNT, F		;da bi dobili LINE (X)1
	RLF COUNT, F		;koristimo formulu
	BCF COUNT, 00h		;COUNT=30h+(X-1)*2
	MOVLW 30h
	ADDWF COUNT, F

	MOVF COUNT, W
	MOVWF FSR		;prebacujemo COUNT u FSR

	MOVF INDF, W		;citamo vrijednost iz INDF

RETURN

Tetris_Table2

	MOVWF COUNT		;koristimo indirektno adresiranje
	RLF COUNT, F		;da bi dobili LINE (X)2
	BCF COUNT, 00h		;koristimo formulu
	MOVLW 01h		;COUNT=30h+2X-1
	SUBWF COUNT, F
	MOVLW 30h
	ADDWF COUNT, F

	MOVF COUNT, W
	MOVWF FSR		;prebacujemo COUNT u FSR

	MOVF INDF, W		;citamo vrijednost iz INDF

RETURN

;Tablice za iscitavanje vrijednosti stupaca

Tetris_BitTable

	MOVWF BIT_COUNT
	ADDWF BIT_COUNT, F
	ADDWF BIT_COUNT, W
	ADDWF PCL, F
	
	BTFSC CURR_PT1, 00h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 01h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 02h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 03h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 04h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 05h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 06h
	RETLW B'11110000'
	RETLW B'00000000'

	BTFSC CURR_PT1, 07h
	RETLW B'11110000'
	RETLW B'00000000'

RETURN

;Tablica, tj. procedura za crtanje naslova

Tetris_LoadTitle

	MOVLW B'01000000'
	MOVWF LINE011

	MOVLW B'01111110'
	MOVWF LINE021

	MOVLW B'01000000'
	MOVWF LINE031

	MOVLW B'00111110'
	MOVWF LINE041

	MOVLW B'00101010'
	MOVWF LINE051

	MOVLW B'01000000'
	MOVWF LINE061

	MOVLW B'01111110'
	MOVWF LINE071

	MOVLW B'01000000'
	MOVWF LINE081

	MOVLW B'00011110'
	MOVWF LINE091

	MOVLW B'00010000'
	MOVWF LINE101

	MOVLW B'00000000'
	MOVWF LINE111

	MOVLW B'01011110'
	MOVWF LINE121

	MOVLW B'00000000'
	MOVWF LINE131

	MOVLW B'00101110'
	MOVWF LINE161

	MOVLW B'00101010'
	MOVWF LINE151

	MOVLW B'00111010'
	MOVWF LINE141

	MOVLW B'00000000'
	MOVWF LINE171

	MOVLW B'00000010'
	MOVWF LINE181

	MOVLW B'00000000'
	MOVWF LINE191

	MOVLW B'00000010'
	MOVWF LINE201

	MOVLW D'20'
	MOVWF FOURTIME

	MOVLW B'01000000'
	MOVWF CURR_PT1

	MOVLW HIGH Tetris_TableModify2
	MOVWF PCLATH

IntroLoop

	MOVF FOURTIME, W
	CALL Tetris_TableModify2
	DECFSZ FOURTIME, F
	GOTO IntroLoop

RETURN

Tetris_ClearTable

	MOVLW D'24'
	MOVWF FOURTIME

	CLRF CURR_PT1

	MOVLW HIGH Tetris_TableModify
	MOVWF PCLATH

ClearLoop

	MOVF FOURTIME, W
	CALL Tetris_TableModify

	MOVF FOURTIME, W
	CALL Tetris_TableModify2

	DECFSZ FOURTIME, F
	GOTO ClearLoop

RETURN

Tetris_TableModify
;W - broj linije, CURR_PT1 - nova vrijednost

	MOVWF COUNT		;koristimo indirektno adresiranje
	DECF COUNT, F		;da bi dobili LINE (X)1
	RLF COUNT, F		;koristimo formulu
	BCF COUNT, 00h		;COUNT=30h+(X-1)*2
	MOVLW 30h
	ADDWF COUNT, F

	MOVF COUNT, W
	MOVWF FSR		;prebacujemo COUNT u FSR

	MOVF CURR_PT1, W
	MOVWF INDF		;stavljamo vrijednost u INDF

RETURN

Tetris_TableModify2
;W - broj linije, CURR_PT1 - nova vrijednost

	MOVWF COUNT		;koristimo indirektno adresiranje
	DECF COUNT, F
	RLF COUNT, F		;da bi dobili LINE (X)2
	BCF COUNT, 00h		;koristimo formulu
	INCF COUNT, F		;COUNT=30h+(X-1)*2+1
	MOVLW 30h
	ADDWF COUNT, F

	MOVF COUNT, W
	MOVWF FSR		;prebacujemo COUNT u FSR

	MOVF CURR_PT1, W
	MOVWF INDF		;stavljamo vrijednost u INDF

RETURN

NapraviOnajKojiNeRadi

	CLRF BUFFPLAYLINE04
	CLRF BUFFPLAYLINE03

	MOVLW B'01000000'
	MOVWF BUFFPLAYLINE02
	MOVLW B'11100000'
	MOVWF BUFFPLAYLINE01

	RETURN

END