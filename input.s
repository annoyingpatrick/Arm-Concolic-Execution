    .global main
    
main:
    push    {ip, lr}
    mov r0, #5      /* First argument */
    str r0, [sp]    /* Store r0 at stack location pointed by sp */
    sub sp, sp, #4  /* Decrement stack pointer by 4 bytes */
    mov r1, #3      /* Second argument */
    add sp, sp, #4  /* Increment stack pointer by 4 bytes */
    ldr r0, [sp]    /* Load r0 from stack location pointed by sp */
    bl  add         /* Call add function */
    pop {ip, lr}
    bx  lr          /* Return from main */

add:
    push {r0, r1, lr} 
    bl  multiply     /* Call multiply function */
    add r0, r0, r1  /* Add the arguments */
    pop {r0, r1, lr} 
    bx  lr          /* Return from add */

multiply:
    mul r0, r0, r1  /* Multiply the arguments */
    bx  lr          /* Return from multiply */