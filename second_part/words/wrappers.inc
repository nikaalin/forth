native 'read', read
  pop rdi
  call read_word
  push rdx
  jmp next

native 'prints', prints
  pop rdi
  call print_string
  jmp next

native 'printnl', printnl
  call print_newline
  jmp next

native 'parsei', parsei
  call parse_int
  push rax
  push rdx
  jmp next

native 'compare', compare
  pop rdi
  pop rsi
  call string_equals
  push rax
  jmp next

native 'key', key
  call read_char
  push rax
  jmp next

native 'emit', emit
  pop rdi
  call print_char
  jmp next

native 'save_lw', save_lw
  push last_word
  jmp next

;-------------------------

native 'cfa', cfa
  pop rdi
	call cfa
  push rax
  jmp next

native 'find', find
  pop rdi
  call find_word
  push rax
  jmp next
