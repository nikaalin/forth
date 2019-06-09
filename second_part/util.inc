section .text

find_word:
  xor eax, eax
  mov rsi, [last_word]

  .loop:
    push rdi
    push rsi
    add rsi, link_size
    call string_equals
    pop rsi
    pop rdi

    test rax, rax
    jnz .found
    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop

    xor eax, eax
    ret

  .found:
    mov rax, rsi
    ret

cfa:
  xor eax, eax
  add rdi, link_size
  push rdi
  call string_length
  pop rdi
  add rax, 1
  add rax, 1
  add rdi, rax
  mov rax, rdi
  ret

next:
  mov w, [pc]
  add pc, 8
  jmp [w]
