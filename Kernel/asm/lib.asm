GLOBAL _force_context_switch
GLOBAL _context_switch

EXTERN schedule

SECTION .text

%macro pushState 0
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

%macro popState 0
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

%macro clearStack 0
	pop rax
	pop rax
	pop rax
	pop rax
	pop rax
	pop rax
	pop rax
	pop rax
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

_context_switch:
	pushState
    mov rdi, rsp
    ;mov rsp, privateStack evans
    call schedule
    mov rsp, rax
    popState

    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al

    iretq

return_code:
    ret


SECTION .bss
	privateStack resb 4096
