mutex_lock:
	mov edx, 0
	mov eax, 0 ;edx:eax = UNLOCKED 	= 0
	mov ecx, 0
	mov ebx, 1 ;ecx:ebx = LOCKED 	= 1

	cmpxchg8b [rdi] ;if(edx:eax == [rdi])
					;	set [rdi] to LOCKED
					; 	and jump to was_locked
	jz was_unlocked

was_locked:
	mov rax, 1 ;LOCKED = 1, wasLocked = TRUE
	ret

was_unlocked:
	mov rax, 0 ;UNLOCKED = 0, wasLocked = FALSE
	ret
