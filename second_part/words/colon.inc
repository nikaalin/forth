colon "'", tick, 1
  dq xt_buffer
  dq xt_read
  dq xt_drop
  dq xt_buffer
  dq xt_find, xt_dup
  branchif0 .no_such_word
  dq xt_cfa
  dq xt_pushmode
  branchif0 .interpret
  dq xt_lit, xt_lit
  dq xt_comma
  dq xt_comma
  .interpret:
    dq xt_exit
    .no_such_word:
      dq xt_warn
      dq xt_exit

colon 'double', double
  dq xt_dup
  dq xt_plus
  dq xt_exit

colon '>', greater
  dq xt_swap
  dq xt_less
  dq xt_exit

;----------------------------------------------
; Слова для создания собственных слов = функций
colon ':', colon
  .restart:
  dq xt_buffer
  dq xt_read

  branchif0 .restart
  dq xt_lit, 0          ; interpreter mode flag
  dq xt_buffer
  dq xt_create
  dq xt_lit, 1, xt_pushmode_addr
  dq xt_write

  dq xt_lit, docol_impl
  dq xt_comma

  dq xt_exit

colon ';', semicolon, 1
  dq xt_lit, 0, xt_pushmode_addr
  dq xt_write
  dq xt_lit, xt_exit
  dq xt_comma
  dq xt_exit
