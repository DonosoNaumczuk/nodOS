GLOBAL read_port
GLOBAL write_port

section .text

read_port:
	push rbp
	mov rbp, rsp
	mov rax, 0
	push rdx
	mov rdx, rdi
	in al, dx
	pop rdx
	pop rbp
	ret

write_port:
	push rbp
	mov rbp, rsp
	push rdx
	push rax
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	pop rax
	pop rdx
	pop rbp
	ret
