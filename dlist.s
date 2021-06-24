.section .data, "aw"

.global dlist
dlist:
    .balign 1024
.rept 64
    .byte 0x00
.endr
.global dlist1
dlist1:
.rept 176
    .byte 0x5e
    .byte 0
    .byte 0
.endr
    .byte 0x41
    .byte 0
    .byte 0

.global dlist_end
dlist_end:
