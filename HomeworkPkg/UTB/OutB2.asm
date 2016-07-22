
text SEGMENT
_OutB2 PROC PUBLIC
	push bp
	mov bp,sp
	mov dx, 0b2h
	mov al, 90h
	out dx, al
	pop bp
	ret
_OutB2 ENDP
text ENDS
END
