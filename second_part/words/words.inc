native 'buf', buf
  push qword user_buf
  jmp next

native 'drop', drop
  add rsp, 8
  jmp next

native 'swap', swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next

native 'dup', dup
  push qword [rsp]
  jmp next

native 'rot', rot
  pop rax
  pop rdx
  pop rcx
  push rdx
  push rcx
  push rax
  jmp next

native '.', dot
  pop rdi
  call print_int
  jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

native '!', write
  pop rax
  pop rdx
  mov [rax], rdx
  jmp next

native '@', fetch
  pop rax
  push qword[rax]
  jmp next

native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native 'pushmode_addr', pushmode_addr
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native 'isimmediate', isimmediate
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'initcmd', initcmd
  pop rax
  push pc
  mov [program_stub], rax
  mov pc, program_stub
  mov w, [pc]
  pop pc
  jmp [w]

native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next

native '-', minus
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next

native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next

native '/', div
  pop rax
  pop rcx
  cqo
  idiv rcx
  push rax
  jmp next

native "%", mod
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rdx
  jmp next

native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next
native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'or', or
  pop rax
  or [rsp], rax
  jmp next


native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next

native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .nequals:
  xor rax, rax
  push rax
  jmp next
native '>r', to_r
  pop rax
  rpush rax
  jmp next

native 'r>', from_r
  rpop rax
  push rax
  jmp next

native "r@", r_fetch
  push qword [rstack]
  jmp next
