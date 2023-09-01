cpu z80


section code

pubproc _start
nop
hlt
add A,A
add a,b
add a,c
ld a, b

bit 1, a
bit 7, a

rl a

im 1
im 2
im 0
endp

end
