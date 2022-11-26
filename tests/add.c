int main() {
    return 1 + 2;
}

.global mainfun 
mainfun: 
pushq %rbp 
movq %rsp, %rbp 
movl $4, %eax 
movl %eax, -12(%rbp) 
movl %eax, -8(%rbp) 
movl -8(%rbp), %eax 
movl -12(%rbp), %ebx 
imul $3, %eax, %eax