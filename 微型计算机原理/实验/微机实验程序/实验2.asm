;基础
datas segment
    NUM DB 5,-4,0,3,100,-51
    BUF1 DB 'x>0,$'
    BUF2 DB 'x=0,$'
    BUF3 DB 'x<0,$'
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax

    MOV CX,6
    LEA SI,NUM
DO: MOV BL,[SI]
	CMP BL,0
	JZ TOP1
	JG TOP2
	MOV DX,OFFSET BUF3
	MOV AH,9
	INT 21H

NEXT:
	INC SI
    LOOP DO
    MOV AH,4CH
    INT 21H
    RET
TOP1:
	MOV DX,OFFSET BUF2
	MOV AH,9
	INT 21H
	JMP NEXT
TOP2:
	MOV DX,OFFSET BUF1
	MOV AH,9
	INT 21H
	JMP NEXT

codes ends
    end start


;提高
datas segment
    NUM DB 5,-4,0,3,100,-51
    BUF1 DB 'x>0',0AH,'$'
    BUF2 DB 'x=0',0AH,'$'
    BUF3 DB 'x<0',0AH,'$'
    t1 db 30H,0AH,'$'
    t2 db 30H,0AH,'$'
    t3 db 30H,0AH,'$'
datas ends
stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax

    MOV CX,6
    LEA SI,NUM
DO: MOV BL,[SI]
	CMP BL,0
	JZ TOP1
	JG TOP2
	INC [t3]
	MOV DX,OFFSET BUF3
	MOV AH,9
	INT 21H

NEXT:
	INC SI
    LOOP DO
    LEA DX,T1
    MOV AH,9
    INT 21H
    LEA DX,T2
    MOV AH,9
    INT 21H
    LEA DX,T3
    MOV AH,9
    INT 21H
    MOV AH,4CH
    INT 21H
    RET
TOP1:
	MOV DX,OFFSET BUF2
	MOV AH,9
	INT 21H
	INC [t2]
	JMP NEXT
TOP2:
	MOV DX,OFFSET BUF1
	MOV AH,9
	INT 21H
	INC [t1]
	JMP NEXT

codes ends
    end start


