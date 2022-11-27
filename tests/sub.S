.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $1, %eax
    movl $2, %ebx
    subl %ebx, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
