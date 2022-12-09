.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $2, %eax
    movl $2, %ebx
    movl $0, %ecx
    cmpl %ecx, %ebx
    setg %bl
    cmpl %ebx, %eax
    sete %al
    movq %rbp, %rsp
    popq %rbp
    ret
