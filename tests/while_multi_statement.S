.text
global def i32 mainfun():
    mov i32 %i0, i32 0
    def i32 &a0, i32 %i0
    mov i32 %i0, i32 1
    def i32 &b0, i32 %i0
.LC0:
    mov i32 %i0, i32 &a0
    mov i32 %i1, i32 5
    setl i32 %i0, i32 %i0, i32 %i1
    je .LC1, i32 0, i32 %i0
    mov i32 %i0, i32 &a0
    mov i32 %i1, i32 2
    add i32 %i0, i32 %i0, i32 %i1
    mov i32 &a0, i32 %i0
    mov i32 %i0, i32 &b0
    mov i32 %i1, i32 &a0
    mul i32 %i0, i32 %i0, i32 %i1
    mov i32 &b0, i32 %i0
    jmp .LC0
.LC1:
    mov i32 %i0, i32 &a0
    ret i32 %i0
