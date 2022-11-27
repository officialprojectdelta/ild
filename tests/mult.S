.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl $3, %ebx
    imul %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
