;基础
datas segment
    NA DB 'MY NAME IS HUANGYI.$'
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax
    MOV CX,03H
    MOV DX,OFFSET NA
    MOV AH,09H
    INT 21H
    mov ah,4ch
    int 21h
codes ends
    end start

;提高
datas segment
    NA DB 'MY NAME IS $'
    OBUF DB 100,?,100 DUP(?)
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
	push ds
	xor ax,ax
	push ax
    mov ax,datas
    mov ds,ax
    MOV CX,03H
    MOV DX,OFFSET NA
    MOV AH,09H
    INT 21H
    LEA DX,OBUF
    MOV AH,0AH
    INT 21H
    mov ah,4ch
    int 21h
codes ends
    end start

;创新
datas segment
    NA DB 0AH,'WHAT IS YOUR NAME ?',0AH,'$'
    OBUF DB 100,?,100 DUP(?)
    IBUF DB 'HI,$'
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax
    MOV CX,0003H
DO: MOV DX,OFFSET NA
    MOV AH,09H
    INT 21H
    MOV DX,OFFSET OBUF
    MOV AH,0AH
    INT 21H
    MOV BL,OBUF+1
    MOV BH,0
    LEA SI,OBUF+2
    MOV BYTE PTR [BX+SI],'$'
    MOV DX,OFFSET IBUF 
    MOV AH,09H
    INT 21H 
    MOV DX,SI
    MOV AH,09H
    INT 21H
    LOOP DO
    mov ah,4ch
    int 21h
codes ends
    end start

