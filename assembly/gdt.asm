; The GDT table.
gdt_table:

; First 8 bytes of the table must be null, to prevent errors.
null_descriptor:
	dd 0x0, 0x0 ; Two sets of 4 bytes padded with zeroes.

; Defines a 1MB code segment in memory.
gdt_code_entry:
	dw 0xffff ; First part of the limit description.
	dw 0x0 ; First part of the base description.
	db 0x0
	db 10011010b ; Various flags.
	db 11001111b ; More flags, and second part of limit description.
	db 0x0 ; Second part of base description.

; Defines a 1MB data segment in memory, overlaps the code segment.
gdt_data_entry:
	dw 0xffff ; First part of the limit description.
	dw 0x0 ; First part of the base description.
	db 0x0
	db 10010010b ; Various flags.
	db 11001111b ; More flags, and second part of limit description.
	db 0x0 ; Second part of base description.

gdt_table_end:

; Information for the CPU to know about the GDT.
gdt_descriptor:
	dw gdt_table_end - gdt_table - 1 ; The size of the GDT in memory.
	dd gdt_table ; Start address of the GDT.

%define CODE_SEGMENT gdt_code_entry - gdt_table
%define DATA_SEGMENT gdt_data_entry - gdt_table
