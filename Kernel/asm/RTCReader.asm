GLOBAL	getHour
GLOBAL	getMinutes
GLOBAL	getSeconds
GLOBAL	getDay
GLOBAL	getMonth
GLOBAL	getYear

section .text

getHour:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	04h
	out		70h, al
	in		al, 71h

	leave
	ret

getMinutes:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	02h
	out		70h, al
	in		al, 71h

	leave
	ret

getSeconds:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	00h
	out		70h, al
	in		al, 71h

	leave
	ret

getDay:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	07h
	out		70h, al
	in		al, 71h

	leave
	ret

getMonth:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	08h
	out		70h, al
	in		al, 71h

	leave
	ret

getYear:
	push	rbp
	mov		rbp, rsp

	mov		rax,00h
	mov		al,	09h
	out		70h, al
	in		al, 71h

	leave
	ret
