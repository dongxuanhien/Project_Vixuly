// Cau hinh chan
data_7seg equ p0
led2 bit p2.1
led3 bit p2.2
led4 bit p2.3
//data_adc equ p1

org 00h
ljmp START				  ; bo qua bang vector ngat

// Trinh phuc vu ngat ngoai 1
org 0013H
ljmp INTR
VE0:	
reti	   

// Trinh phuc vi ngat Timer 1
org 001Bh
ljmp Timer1
VE:
reti

// Setup che do ngat
org 23h
START:
mov IE,#10001100B					   ; Ngat ngoai tai INT1, suon xuong	. va ngat Timer 1
setb IT1   	 	 

// Setup che do Timer 1
mov TMOD,#00010000B                  ; Che do Timer 1 16 bit(Mode1)
mov TH1,#000h				   ; Nap gia tri bat dau 16bit
mov TL1,#000h
setb TR1

mov 0FEh,#140                   ; Bien dem Timer
mov 0FDh,#100				   ; FDh thanh ghi luu tru nguong
MOV p1,#0FFH            ; chon p1 là cong dau vao

lcall hihi1							; doc ADC

/* Hien thi */
DISPLAY:
// Code kiem tra nguong
mov a,0FCh
cjne a,0FDh,check			   ; Check
sjmp main				   ; p1=r2 nhay den main
check:
jnc main				  ; C=0 p1>r2
                 		  ; C=1 p1<r2		 

// Check buzzer
cjne r6,#0,checkReset

main0:						    // Code hien thi data < nguong
clr p2.4
mov r6,#0
lcall hihi0						 ; hien thi
ljmp DISPLAY
	
// Code hien thi data > nguong	 + Nhap nhay
main:  			   ; hien so theo phuong phap quet Led
setb p2.4
mov r6,#1	
lcall hihi0						   ; hien thi
lcall delay_lau											
lcall delay	  
ljmp DISPLAY   

// Check Reset
checkReset:
jb p2.0,main
ljmp main0

//CT con
Timer1:
    // Doc lai gia tri + Reset bien dem
djnz 0FEh,SKIP						; Check 10s
mov 0FEh,#140						; Nap lai bien dem
lcall hihi2							; Nap lai timer
lcall hihi1		 					; DOC adc
ljmp VE    		

   // Nap lai gia tri cho bo dem
SKIP:
lcall hihi2							 ; nap lai timer
ljmp VE

    // Hien thi
hihi0:
mov dptr,#ma7seg
setb led2						; Bat Led so 2
//clr p1.0
mov a,r3
movc a,@a+dptr
mov data_7seg,a
lcall delay
clr led2						; Tat Led so 2
setb led3						; Bat Led so 3
mov a,r4
movc a,@a+dptr
mov data_7seg,a
lcall delay
clr led3						; Tat Led so 3
setb led4						; Bat Led so 4
mov a,r5
movc a,@a+dptr
mov data_7seg,a
lcall delay
clr led4						; Tat Led so 4
ret
	// Doc ADC
hihi1:
   //clr p2.7				 ; THEM
   //setb p2.7			 ; THEM
   clr p2.6             ; Dat WR=0
   setb p2.6            ; Dat WR=1 de bat dau chuyen doi
  
HERE:
   jb p2.7,HERE         ; Cho ket thuc chuyen doi
   clr p2.5             ; Ket thuc chuyen doi, cho phep RD
   lcall delay   		; THEM	
   MOV a,p1             ; Doc du lieu vao thanh ghi A
   MOV 0FCh,a
   setb p2.5            ; Dua RD=1 de cho lan doc sau
   setb p2.7            ; THEM


// Code chuyen doi so
PRE:
mov b,#100
div ab
mov r3,a
mov a,b
mov b,#10
div ab
mov r4,a
mov r5,b
ret
	// Nap Timer	
hihi2:
clr TF1
clr TR1
mov TMOD,#00010000B  
mov TH1,#000h				  
mov TL1,#000h
setb TR1
ret   		
    // Delay
delay:							; hàm delay nho
mov r7,#250
djnz r7,$
ret
delay_lau:						; ham delay lau => Nhay led
MOV 0F9H,#60
lap: MOV 0F8H,#250
djnz 0F8H,$
djnz 0F9H,lap
ret	

    // Ngat ngoai
INTR:
inc 0FDh
mov a,0FDh
lcall PRE
lcall hihi0
mov r7,0FDh					  ; Check neu bang 150 thi reset nguong ve 1
cjne r7,#150,CONTINUE
mov 0FDh,#1
CONTINUE:
ljmp VE0		 
ma7seg:
db 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
end

										 