GLOBAL	_getHour
GLOBAL	_getMinutes
GLOBAL	_getSeconds
GLOBAL	_getDay
GLOBAL	_getMonth
GLOBAL	_getYear
GLOBAL	_setBinaryTime

section .text

_setBinaryTime:
	push	rbp
	mov		rbp, rsp

	mov rax,0
	mov al,0Bh
	out 70h,al
	in al,71h
	or al,4
	out 71h,al
	
	leave
	ret

_getHour:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	04h
	out		70h, al
	in		al, 71h

	leave
	ret

_getMinutes:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	02h
	out		70h, al
	in		al, 71h

	leave
	ret

_getSeconds:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	00h
	out		70h, al
	in		al, 71h

	leave
	ret

_getDay:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	07h
	out		70h, al
	in		al, 71h

	leave
	ret

_getMonth:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	08h
	out		70h, al
	in		al, 71h

	leave
	ret

_getYear:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	09h
	out		70h, al
	in		al, 71h

	leave
	ret
