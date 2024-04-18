@ COMPILER GENERATED
@ int add(int x, int y)
@ {
@   return x+y;
@ }

@ int main()
@ {
@   int x = 5;
@   int y = 3;

@   int z = add(x, y);
@ }


add:
    push    {r7}
    sub     sp, sp, #12
    add     r7, sp, #0
    str     r0, [r7, #4]
    str     r1, [r7]
    ldr     r2, [r7, #4]
    ldr     r3, [r7]
    add     r3, r3, r2
    mov     r0, r3
    adds    r7, r7, #12
    mov     sp, r7
    ldr     r7, [sp], #4
    bx      lr
main:
    push    {r7, lr}
    sub     sp, sp, #16
    add     r7, sp, #0
    movs    r3, #5
    str     r3, [r7, #12]
    movs    r3, #3
    str     r3, [r7, #8]
    ldr     r1, [r7, #8]
    ldr     r0, [r7, #12]
    bl      add
    str     r0, [r7, #4]
    movs    r3, #0
    mov     r0, r3
    adds    r7, r7, #16
    mov     sp, r7
    pop     {r7, pc}