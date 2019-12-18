;基础
datas segment
    OBUF DB 'WELCOME TO OUR SCHOOL',0AH,'$' 
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax
    
    XOR BX,BX;清BX
    LEA SI,OBUF
    MOV DX,SI
    MOV AH,9
    INT 21H
DO: MOV AL,[SI]
    CMP AL,'$'
    JZ NEXT
    INC SI
    CMP AL,'A'
    JNC T1;AL比'A'更大
    JMP DO
NEXT: 
	LEA DX,OBUF
	MOV AH,9
	INT 21H
	ADD BL,30H
	MOV DL,BL
	MOV AH,2
	INT 21H
    mov ah,4ch
    int 21h
    ret
T1:
    CMP AL,'Z'
    JC T2
    JMP DO
T2:
	ADD AL,32
	DEC SI
	MOV [SI],AL
	INC SI
	INC BL
	JMP DO
	
codes ends
    end start



;创新
datas segment
    OBUF DB 100,?,100 DUP(?)
    TEMP1 DB 0AH,0DH,'IN[#]:$'
    TEMP2 DB 0AH,0DH,'OUT[#]:$'
    TEMP3 DB 0AH,0DH,'      :$'
datas ends

stacks segment
    dw 32 dup(?)
stacks ends

codes segment
    assume cs:codes,ds:datas,ss:stacks
start:
    mov ax,datas
    mov ds,ax
    
    LEA DX,TEMP1+1
    MOV AH,9
    INT 21H
RETRY:
    XOR BX,BX;清BX
    LEA DX,OBUF
    MOV AH,0AH
    INT 21H
    LEA SI,OBUF+2
    MOV BL,OBUF+1
    MOV BYTE PTR [SI+BX],'$';	串尾送$'
	XOR BL,BL;	计数
DO: MOV AL,[SI]
    CMP AL,'$'
    JZ NEXT
    INC SI
    CMP AL,'A'
    JNC T1;AL比'A'大
    JMP DO
NEXT:
	LEA DX,TEMP2
    MOV AH,9
    INT 21H
	LEA DX,OBUF+2
	MOV AH,9
	INT 21H
	MOV AL,BL
	MOV AH,0
	MOV BL,10
	DIV BL;	除10，取出个位和十位
	MOV BX,3030H
	ADD BH,AL
	ADD BL,AH
	LEA DX,TEMP3
	MOV AH,9
	INT 21H
	MOV DL,BL
	MOV AH,2
	INT 21H
	LEA DX,TEMP1
    MOV AH,9
    INT 21H
	JMP RETRY
    mov ah,4ch
    int 21h
    ret
T1:
    CMP AL,'Z'
    JC T2
    JMP DO
T2:
	SUB AL,32;小写改大写
	DEC SI
	MOV [SI],AL
	INC SI
	INC BL
	JMP DO

codes ends
    end start
	



