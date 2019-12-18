;基础
datas segment
    SB DW 65535
    HASCT DB '0123456789ABCDEF'
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax
    MOV BX,SB
	CALL PRINT
	MOV AH,4CH
    INT 21H
PRINT:
	MOV CH, 4
S1:
    MOV CL, 4
    ROL BX, CL
    MOV AL, BL
    AND AX, 000FH
	MOV SI, AX
    MOV DL, HASCT[SI]
    MOV AH, 02H
    INT 21H
    DEC CH
    JNZ S1
RET    
codes ends
    end start



;创新
data segment
	IBUF DB 0DH,'INPUT A DICEMAL NUMBER <65535 END WITH ENTER',0AH,'$'
	ERR DB 0DH,'INPUT IS ERROR',0AH,'$'
data ends

code segment
    assume cs:code,ds:data
start:
	mov ax,data
	mov ds,ax
    CALL READ
    CALL CHANGE
    CALL CRLF
    MOV AH,4CH
    INT 21H

READ: 
	LEA DX, IBUF
	MOV AH, 9
	INT 21H
	MOV BX, 0
NEWCHAR:
    MOV AH, 1
    INT 21H
    CMP AL,0DH
    JZ NEXT
    SUB AL, 30H
    JL EXIT
    CMP AL, 9
    JG EXIT
    CBW
    XCHG AX, BX
    MOV CX, 10
    MUL CX
    XCHG BX, AX
    ADD BX, AX
    JMP NEWCHAR
EXIT:
	LEA DX,ERR
	MOV AH,9
	INT 21H
NEXT:
	RET
CHANGE:
	MOV CH, 4
S1:
    MOV CL, 4
    ROL BX, CL
    MOV AL, BL
    AND AL, 0FH
    ADD AL, 30H
    CMP AL, 3AH
    JL PRINT
    ADD AL, 07H
PRINT:
    MOV DL, AL
    MOV AH, 02H
    INT 21H
    DEC CH
    JNZ S1
	RET

CRLF:
    MOV DL, 0DH
    MOV AH, 2
    INT 21H
    MOV DL, 0AH
    MOV AH, 2
    INT 21H
	JMP START
    RET
code ends
    end start

