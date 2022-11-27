.global foofun
foofun:
    pushq %rbp
    movq %rsp, %rbp
    movl $3, %eax
    movq %rbp, %rsp
    popq %rbp
    ret
.global mainfun
mainfun:
    pushq %rbp
    movq %rsp, %rbp
    movl $5, %eax
    movl -8(%rbp), %eax
    movq %rbp, %rsp
    popq %rbp
    ret
