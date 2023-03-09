locals @@
.model tiny
.code

org 100h

start:
	mov dx, offset Answer
	mov ah, 0ah
	int 21h

	mov si, offset Answer + 2
	mov di, offset RightAnswer
	call Strcmp

	cmp ax, 00h
	je @@correct
	mov ah, 09h
	mov dx, offset uncorrectMessage
	int 21h
	jmp @@countinue

@@correct:	
	mov ah, 09h
	mov dx, offset correctMessage
	int 21h
	jmp @@countinue

	

@@countinue:
	mov ax, 4c00h	
	int 21h

nop
Answer db 250d, 20 dup (0)
nop
RightAnswer db "VOVA KRASAVA", 0dh, 00
correctMessage db "Congrats, access confirmed$"
uncorrectMessage db "Access denied$"

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


;------------------------------------------------
; Hash function
;------------------------------------------------
; Entry: si = pointer on str with '$' terminate symbol
; Exit: ax = Hash
; Destoys: bx
;------------------------------------------------
HashFunc 		proc

	xor bx, bx
	xor ax, ax
	xor dx, dx

@@MainLoop:
	cmp byte ptr [si], 0dh
	je @@countinue

	mov bl, [si]
	shr bx, 1
	shl bx, 2
	xor bx, 10101b
	add ax, bx
	inc si
	jmp @@MainLoop


@@countinue:

ret 
endp
;------------------------------------------------

end start
