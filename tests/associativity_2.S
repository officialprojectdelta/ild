.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $6, %eax
    movl $3, %ebx
    movq %rax, %rcx
    movslq%eax, %rax
    cqo
    idivl %ebx
    movl $2, %ebx
    movq %rax, %rcx
    movslq%eax, %rax
    cqo
    idivl %ebx
    movq %rbp, %rsp
    popq %rbp
    ret
