native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'lit', lit
    push qword [pc]
    add pc, 8
    jmp next

native 'pushlit', pushlit
  push xt_lit
  jmp next

native 'unsetbranch', unsetbranch
  mov byte[was_branch], 0
  jmp next

native 'saveword', saveword
  pop rax
  mov  qword[here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  add qword[here], rax
  jmp next

native 'savenum', savenum
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  mov qword[here], rax
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next

native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'comp_m', comp_m
  mov  rdi, cmode_message
  call print_string
  call print_newline
  jmp next

native 'inte_m', inte_m
  mov  rdi, imode_message
  call print_string
  call print_newline
  jmp next

native 'branchifz', branchifz
  pop rax
  test rax, rax
  jnz .skip
  mov pc, [pc]
  jmp next
  .skip:
    add pc, 8
    jmp next

native 'branch', branch
    mov pc, [pc]
    jmp next

native 'docol', docol
  sub rstack, 8
  mov [rstack], pc
  add w, 8
  mov pc, w
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'create', create
  ; генерация ссылки
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
  ;mov byte [rsi], 0
  ;inc rsi

  ; генерация имени
  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  ; флаги
  inc rsi
  pop rax
  mov [rsi], al
  inc rsi

  mov [here], rsi
  jmp next


native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

native 'warn', warn
  mov  rdi, warning_message
  call print_string
  call print_newline
  mov  pc, xt_loop
  jmp next

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next
