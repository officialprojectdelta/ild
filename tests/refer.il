.text
global def i32 mainfun ():
    mov i32 %i0, i32 2
    def i32 &a0, i32 %i0
    mov i32 %i0, i32 &a0
    ret i32 %i0

