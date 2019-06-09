section .data
  xt_run: dq run
  xt_loop: dq main_loop
  program_stub: dq 0
  warning_message: db "Warning: no such word", 0
  imode_message: db "Interpreter mode", 0
  cmode_message: db "Compiler mode", 0
  mode: dq 0
  was_branch: db 0
  here: dq forth_mem
  stack_start: dq 0


section .bss
  resq 1023
  rstack_start: resq 1
  forth_mem: resq 65536
  input_buf: resb 1024
  user_buf: resb 1024

%include 'io_lib.inc'
%include 'macro.inc'
%include 'util.inc'
%include 'words/dict.inc'

const here, [here]

global _start

section .data
  last_word: dq link

section .text
_start:
  mov rstack, rstack_start
  mov stack, stack_start
  mov qword[mode], 0
  mov pc, xt_run
  cmp qword [stack_start], 0
  je  .first
  mov rsp, [stack_start]
  jmp next
  .first:
    mov [stack_start], rsp
  jmp next

run:
  dq docol_impl

main_loop:
    dq xt_buffer
    dq xt_read
    branchif0 exit
    dq xt_buffer
    dq xt_find

    dq xt_pushmode
    branchif0 .interpreter_mode

  .compiler_mode:
    ;dq xt_comp_m
    dq xt_dup
    branchif0 .compiler_number

    dq xt_cfa

    dq xt_isimmediate
    branchif0 .notImmediate

    .immediate:
      dq xt_initcmd
      branch main_loop

    .notImmediate:
      dq xt_isbranch
      dq xt_comma
      branch main_loop

    .compiler_number:
      dq xt_drop
      ;dq xt_buffer
      dq xt_parsei
      branchif0 .warning
      dq xt_wasbranch
      branchif0 .lit

      dq xt_unsetbranch
      dq xt_savenum
      branch main_loop

  	  .lit:
      dq xt_lit, xt_lit
      dq xt_comma
      dq xt_comma

      branch main_loop


  .interpreter_mode:
    ;dq xt_inte_m
    dq xt_dup
    branchif0 .interpreter_number

    dq xt_cfa
    dq xt_initcmd
    branch main_loop

    .interpreter_number:
      dq xt_drop
      ;dq xt_buffer
      dq xt_parsei
      branchif0 .warning
      branch main_loop

  .warning:
    dq xt_drop
  	dq xt_warn
    branch main_loop

exit:
  dq xt_bye
