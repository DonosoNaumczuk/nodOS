GLOBAL _force_context_switch
GLOBAL _context_switch

EXTERN schedule

SECTION .text

%macro pusheq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro popeq 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

_force_context_switch:
    mov rax,rsp

    push QWORD 0
    push QWORD 0
    push rax
    pushfq
    push QWORD 0x008
    push return_code

    pusheq

_context_switch:
    mov rdi, rsp
    mov rsp, privateStack
    call schedule
    mov rsp, rax

    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al

    popeq
    iretq

return_code:
    ret

SECTION .bss
	privateStack resb 4096

SECTION .data
