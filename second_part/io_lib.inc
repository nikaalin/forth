%define BUFFER_SIZE 32

section .data
	char_diff: db 0x00
	buf : times 256 db 0

section .text

string_length:
    xor rax, rax ; обнуляем счетчик
    .loop:
      cmp  byte[rdi + rax], 0 ; вычисляется адрес + сравнивается с 0 + устанавливаются флаги
      je   .exit ;смотрим в регистр состояния во флаг equals, если 0 - заканчиваем цикл
      inc  rax ;если 1 - учитываем символ и продолжаем цикл
      jmp  .loop
    .exit:
    ret

print_string:
    call string_length ; помещаем в rax длину строки
    ; готови регистры для системного вызова sys_write
    mov rsi, rdi ; откуда выводить (передаваемая ссылка)
    mov rdi, 1 ; куда выводить (stdout)
    mov rdx, rax ; сколько выводить
    mov rax, 1 ; код sys_write
    syscall
    ret

print_char:
    push rdi ; сохраняем символ на стэк
    mov rsi, rsp ; откуда выводить (адрес вершины стэка)
    mov rdi, 1 ; куда выводить (stdout)
    mov rdx, 1 ; сколько выводить
    mov rax, 1 ; код sys_write
    syscall
    pop rdi ; забираем символ со стэка
    ret

print_newline:
    mov rdi, 10 ; передаем как параметр, код конца строки
    jmp print_char ; выводим его на экран

string_copy:
    call string_length
    mov  rcx, rax ; помещаем длину строки в rcx
    inc  rcx ; увеличиваем, чтобы дойти до 0
    xchg rsi, rdi ; меняем местами (в rdi теперь ссылка на место копирования)
    mov  byte[rdi + rax], 0 ; нультермируем место, куда будем копировать строку
    rep  movsb ; повторяем копирование(побайтово), используя rcx, как счетчик
    ret

print_uint:
    push rbp ; сохраняем зачение rbp, положив его на стэк
    mov  rbp, rsp ; сохраняем в rbp адрес вершины стэка
    mov  rax, rdi ; сохраняем переданное число в "аккумулятор"
    mov  rdi, 10 ; заносим делитель (10) в rdi
    sub  rsp, BUFFER_SIZE ; выделяем буфер в стэке
    dec  rbp ; выделяем место под 0 на вершине стэка (нультермируем для вывода)
    mov  byte[rbp], 0 ; записываем 0 на вершину стэка
      .loop:
        dec  rbp ; сдвигаем метку для новой цифры
        xor  rdx, rdx ; обнуление rdx
        div  rdi ; целочисленное деление на 10 с записью в остатка в rdx
        add  rdx, '0' ; остаток + код первой цифры в ASCII
        mov  byte[rbp], dl ; сохраняем 1 байт (цифру) на стэк
        test rax, rax ; устанавливает флаги (ZF = 0, если rax = 0)
        jnz  .loop ; если rax !=0, продолжает цикл
    mov rdi, rbp ; помещаем аргумент для print_string (метку на стэке)
    call print_string ; выводим число в stdout
    add rsp, BUFFER_SIZE ; освобождаем буфер
    pop rbp ; восстанавливаем значение rbp
    ret

print_int:
  test rdi, rdi ; устанавливаем флаги
  jns  .unsigned ; переход, если беззнаковое
  push rdi ; сохраняем перед вызовом функции
  mov  rdi, '-'  ; помещаем аргумент
  call print_char ; выводим знак
  pop  rdi ; восстанавливаем значение rdi
  neg  rdi ; делаем число положительным (not + inc)
  .unsigned:
  jmp  print_uint ; обычный вывод беззнакового числа

string_equals:
  call string_length
  mov  rcx, rax ; сохраняем длину первой строки в rcx
  xchg rdi, rsi ; помещаем в rdi адрес второй строки
  call string_length
  cmp  rax, rcx ; сравнение длин и установка флага
  jne  .not_equals ; если флаг не равен 0 (не равны), возврат 0
  repe cmpsb ; repe - повторять пока равны; cmpsb - устанавливает флаги
  jne  .not_equals ; если перебор прекратился, флаг ZF будет установлен
  mov rax, 1 ; возврат 1
  ret
  .not_equals:
    mov rax, 0 ; возврат 0
    ret

read_char:
	push 0
	mov rsi, rsp
	mov rdx, 1
	xor rdi, rdi
	xor rax, rax
	syscall
	pop rax
	ret

read_word:
	push r14
  push r15
  xor r14, r14
  mov r15, rsi
  dec r15

  .read_first:
	  push rdi
	  call read_char
	  pop rdi
	  cmp al, ' '
	  je .read_first
	  cmp al, 10
	  je .read_first
	  cmp al, 13
	  je .read_first
	  cmp al, 9
	  je .read_first
	  test al, al
	  jz .read_success

  .read_next:
	  mov byte [rdi + r14], al
	  inc r14

	  push rdi
	  call read_char
	  pop rdi
	  cmp al, ' '
	  je .read_success
	  cmp al, 10
	  je .read_success
	  cmp al, 13
	  je .read_success
	  cmp al, 9
	  je .read_success
	  test al, al
	  jz .read_success
	  cmp r14, r15
	  je .read_err
	  jmp .read_next

  .read_success:
	  mov byte [rdi + r14], 0
	  mov rax, rdi
	  mov rdx, r14
	  pop r15
	  pop r14
	  ret

  .read_err:
	  xor rax, rax
		xor rdx, rdx
	  pop r15
	  pop r14
	  ret

parse_uint:
  call string_length
  mov rcx, rax ; сохраняем в rcx длину строки
  mov rsi, rdi ; сохраняем в rsi адрес строки
  xor rdx, rdx ; обнуление (для хранения колличества цифр)
  xor rax, rax ; обнуление (для хранения полученного числа)
  .pars:
    xor  rdi, rdi ; обнуление
    mov  dil, byte[rsi + rdx] ; берем следующий символ по (адрес + сдвиг)
    cmp  dil, '0' ; сравниваем с 0 и устанавливаем флаги
    jb   .exit ; если код символа меньше 0, заканчиваем перебор
    cmp  dil, '9' ; сравниваем с 9 и устанавливаем флаги
    ja   .exit ; если код символа больше 9, заканчиваем перебор
    sub  dil, '0' ; вычитаем код 0, чтобы получить число от 0 до 9
    imul rax, 10 ; умножаем на 10
    add  rax, rdi ; прибавляем полученную цифру
    inc  rdx ; считаем эту цифру
    dec  rcx ; уменьшаем длину строки + устанавливаем флаги
    jnz  .pars ; если строка не кончилась, тогда всё заново
  .exit:
  ret

parse_int:
  cmp byte[rdi], '-' ; проверяем на наличие минуса (устанавливаются флаги)
  je .minus ; если есть, обрабатываем его
  jmp parse_uint ; если нет, работаем как с обычной строкой
  .minus:
    inc rdi ; пропускаем минус
    call parse_uint ; обрабатываем обычную строку
    test rdx, rdx ; устанавливаем флаги
    jz .null ; если после минуса нет цифр, выходим
    neg rax ; иначе делаем число отрицательным
    inc rdx ; учитываем минус (в колличестве знаков)
    ret
  .null:
    xor eax, eax ; обнуляем ответ
    ret
