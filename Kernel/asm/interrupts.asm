GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL haltCycle
GLOBAL printRegisters

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _systemCallHandler

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscall_dispatcher
EXTERN printWithColor
EXTERN printHexa
EXTERN newLine

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
%endmacro

%macro popState 0
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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

haltCycle:
	cli
	hlt
	jmp haltCycle

printRegisters:
	pushState
	mov rdi,register17
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,0			;nose como leer el rip
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register1
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rax
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register2
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rbx
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register3
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rcx
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register4
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rdx
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register5
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rsi
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register6
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rdi
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register7
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rbp
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register8
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,rsp
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register9
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r8
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register10
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r9
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register11
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r10
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register12
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r11
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register13
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r12
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register14
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r13
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register15
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r14
	call printHexa
	call newLine
	popState

	pushState
	mov rdi,register16
	mov rsi,0x00000005
	mov rdx,0x0000000F
	call printWithColor
	popState

	pushState
	mov rdi,r15
	call printHexa
	call newLine
	popState
	
	ret


picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push rbp
    mov rbp, rsp
    mov ax, di  ; ax = 16 bits mask
    out	0A1h,al
    pop rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

_systemCallHandler:
	call syscall_dispatcher


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1

SECTION .data
	register1 db "RAX: "
	register2 db "RBX: "
	register3 db "RCX: "
	register4 db "RDX: "
	register5 db "RSI: "
	register6 db "RDI: "
	register7 db "RBP: "
	register8 db "RSP: "
	register9 db "R8:  "
	register10 db "R9:  "
	register11 db "R10: "
	register12 db "R11: "
	register13 db "R12: "
	register14 db "R13: "
	register15 db "R14: "
	register16 db "R15: "
	register17 db "RIP: "



