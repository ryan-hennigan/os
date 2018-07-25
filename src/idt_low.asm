; load the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void _idt_load();'
; In turn, the variable '_idtp' is defined in file 'idt.C'.
global _idt_load
extern _idtp
_idt_load:
	lidt [_idtp]
	ret
