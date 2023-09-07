cpu i80

proc _start
call test
mov a, b

mov m, a

rst 0
rst 1
endp

proc test
mov a,b
ret
endp
end