# Spec for IIL 

instructions that move data follow this syntax dest, src1, (src2)
otherwise it is src1, (src2)

every oprand is prefixed by its type, (i8, i16, i32, i64, ui8, ui16, ui32, ui64, f32, f64), and pointers are just ui64

& and then a varname is a memory location (local variable)
%i and then a number is an intermediate location, which could be a register or memory location
however, this will mostly not be used after optimization anyways

sudoassembly makes assembly instructions (mostly) follow the dest src syntax, and some instruction are sligtly modified, but actual operations are used, and any valid assembly is valid sudoassembly

some examples 

int test(void)
{
    int x = 3;
    int y = x * 5 / 2
    float b = 98723 + 3.5;
    
    return x + y - b;
}

would generate 

i32 test():
  mov i32 &x0, ui8 3 // int x = 3; x0 is because of adding the scope it is in in the symtable generator, its a compiler thing
 
  mul i32 %i0, i32 &x0, ui8 5
  div i32 &y0, i32 %i0, ui8 2
  
  add f32 %b0, ui32 98723, f32 3.5
  
  add i32 %i0, i32 &x0, i32 &y0
  sub f32 %i0, i32 &i0, f32 &b0
  
  ret %i0


which would generate this after optimization
  mov i32 &x0, ui8 3 // int x = 3; x0 is because of adding the scope it is in in the symtable generator, its a compiler thing
 
  mul i32 %i0, i32 &x0, ui8 5
  div i32 &y0, i32 %i0, ui8 2
    
  mov f32 &b0, f32 98726.5
  
  add i32 %i0, i32 &x0, i32 &y0
  sub f32 %i0, i32 &i0, f32 &b0
  
  ret %i0


which would generate this sudoassembly

(No memory is needed because there are enough registers to compensate) 

  movl %eax, $3
  movl %ebx, $5
  mull %ebx, %eax
  divl %ebx, %2
  
  movss %xmm0, .LD0(%rip)
  
  
  

which would then be optimized / checked again before outputting


