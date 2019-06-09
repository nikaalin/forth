;%define here r11
%define stack r12
%define rstack r13
%define w r14
%define pc r15
%define link 0

%assign link_size 8
%assign word_size 8

%macro create_link 0
  %%link: dq link
  %define link %%link
%endmacro


%macro native 3
  section .data
    w_ %+ %2:
	    create_link
	    db %1, 0
    db %3
	  xt_ %+ %2:
      dq %2 %+ _impl
  section .text
	 %2 %+ _impl:
%endmacro

%macro native 2
  native %1, %2, 0
%endmacro


%macro colon 3
  section .data
      w_ %+ %2:
      create_link
      db %1, 0
   db %3
      xt_ %+ %2:
      dq docol_impl
%endmacro

%macro colon 2
  colon %1, %2, 0
%endmacro


%macro branchif0 1
  dq xt_branchifz
  dq %1
%endmacro

%macro branch 1
  dq xt_branch
  dq %1
%endmacro

%macro rpush 1
  sub rstack, 8
  mov qword [rstack], %1
%endmacro

%macro rpop 1
  mov %1, qword [rstack]
  add rstack, 8
%endmacro

%macro const 2
%defstr %%__cnst_str %1
native %%__cnst_str, %1
    push qword %2
    jmp next
%endmacro
