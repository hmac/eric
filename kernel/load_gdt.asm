extern gdt_desc
global load_gdt
load_gdt:
  lgdt [gdt_desc]
  ret
