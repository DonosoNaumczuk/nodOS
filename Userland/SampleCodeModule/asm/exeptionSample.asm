GLOBAL invalidop
GLOBAL divide0
GLOBAL overflow


invalidop:
	RSM
	ret

divide0:
	mov ax, 0
	mov al,0
	div al
	ret

overflow:
	mov ax, 0xFFFFFF
	ret
