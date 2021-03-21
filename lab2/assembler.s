.data
print_format:
  .string "%d\n"
error_mes:
  .string "Ошибка: переполнение\n"
A:
  .byte 12
B:
  .byte 18
zero:
  .byte 0
array:
  .byte -13, 12, -14, 76, -19, 22, 19, 18, 7

.text
.globl main
.type main, @function
main:
  mov $0, %cl
  movl $array, %eax
  mov (%eax), %bl

pre_loop:
  cmp A, %bl
  je next
  addl $1, %eax
  mov (%eax), %bl
  jmp pre_loop

next:
  addl $1, %eax
  mov (%eax), %bl

loop:
  cmp B, %bl
  je code_end
  cmp zero, %bl
  jl next
  add %bl, %cl
  jo error
  jmp next

error:
  push $error_mes
  call printf
  addl $4, %esp
  mov $0, %ecx
  mov $0, %ebx
  mov $0, %eax
  ret

code_end:
  mov %cl, (%edx)
  push (%edx)
  push $print_format
  call printf
  addl $8, %esp
  mov $0, %ecx
  mov $0, %ebx
  mov $0, %eax
  mov $0, %edx
  ret
