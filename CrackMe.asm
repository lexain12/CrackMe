
locals @@
.model tiny
.code

org 100h

start:
	mov dx, offset Answer
	mov ah, 0ah
	int 21h

	mov si, offset CheckOnJmp
	mov cx, 26d
	call CheckJmp

	mov si, offset Answer + 2
	call HashFunc


@@Hash:
	cmp ax, word ptr RightAnswer
	je @@correct
	mov ah, 09h
	mov dx, offset uncorrectMessage
	int 21h
	jmp @@countinue

db '$'

@@correct:	
	mov si, offset @@Hash 		; check hash of programm	
	call HashFunc
	cmp ax, 0fcf3h
	jne @@countinue

	mov ah, 09h
	mov dx, offset correctMessage
	int 21h
	jmp @@countinue

	

@@countinue:
	mov ax, 4c00h	
	int 21h


correctMessage db "Congrats, access confirmed$"
uncorrectMessage db "Access denied$", 0dh

;------------------------------------------------
; Strcmp
;------------------------------------------------
; Entry: di = address, si = address
; Exit: ax = substitute
; Destroys: dx, si, di
;------------------------------------------------
Strcmp 		proc

	xor ax, ax

@@Loop:
	cmp byte ptr [di], 00h
	je @@done

	cmp byte ptr [si], 00h
	je @@done

	mov al, byte ptr [di]
	cmp al, [si]
	jne @@done
	
	inc si
	inc di
	jmp @@Loop

@@done:
	mov al, byte ptr [di]
	xor dx, dx
	sub al, [si]


	ret 
	endp
;------------------------------------------------


CheckOnJmp:
Answer db 254d, 10d dup (0), '$'

;------------------------------------------------
; Hash function
;------------------------------------------------
; Entry: si = pointer on str with 0dh terminate symbol
; Exit: ax = Hash
; Destoys: bx
;------------------------------------------------
HashFunc 		proc

	mov cx, 5d
	xor bx, bx
	xor ax, ax
	xor dx, dx

@@MainLoop:
	cmp byte ptr [si], '$'
	je @@countinue
	cmp byte ptr [si], 0dh
	je @@countinue

	mov bl, [si]
	shr bx, 1
	shl bx, 2
	xor bx, 10101b
	shl bx, 5
	add bx, 10101b
	xor bx, 50h
	add bx, [si + 1]
	mul cx
	add ax, bx
	inc si
	jmp @@MainLoop


@@countinue:

ret 
endp
;------------------------------------------------

RightAnswer dw 0f8b2h, 0dh, 00

;------------------------------------------------
; Check jmp
;------------------------------------------------
; Entry: si = address from what check, if jmp exit the prog, cx = number of bytes check
;------------------------------------------------
CheckJmp 	proc
	push ax
	xor ax, ax
@@MainLoop:	

	lodsb
	shr ax, 4
	cmp al, 0Eh
	je @@Exit
	loop @@MainLoop

	pop ax
	ret

@@Exit:
	mov ax, 4c00h
	int 21h

ret
endp
;------------------------------------------------

end start
