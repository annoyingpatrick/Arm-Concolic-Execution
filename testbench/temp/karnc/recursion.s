_Z9factoriali:
        push    {fp, lr}
        add     fp, sp, #4
        sub     sp, sp, #8
        str     r0, [fp, #-8]
        ldr     r3, [fp, #-8]
        cmp     r3, #1
        bne     .L2
        mov     r3, #1
        b       .L3
.L2:
        ldr     r3, [fp, #-8]
        sub     r3, r3, #1
        mov     r0, r3
        bl      _Z9factoriali
        mov     r2, r0
        ldr     r3, [fp, #-8]
        mul     r3, r2, r3
.L3:
        mov     r0, r3
        sub     sp, fp, #4
        pop     {fp, lr}
        bx      lr
main:
        push    {fp, lr}
        add     fp, sp, #4
        sub     sp, sp, #8
        mov     r3, #5
        str     r3, [fp, #-8]
        ldr     r0, [fp, #-8]
        bl      _Z9factoriali
        mov     r3, r0
        out     r3
        nop
        mov     r0, r3
        sub     sp, fp, #4
        pop     {fp, lr}
        bx      lr