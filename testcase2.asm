 adj 0           ; Initialize SP to 0
    ldc var1        ; Load var1 (10) into A
    stl 0           ; Store var1 in memory location 0

    ldc var2        ; Load var2 (5) into A
    stl 1           ; Store var2 in memory location 1

    ldc 0           ; Load 0 into A (initial result is 0)
    stl 2           ; Store the initial result in memory location 2

    ldl 1           ; Load var2 (number of times to add) into A
    brz done        ; 

multiply_loop:
    ldl 2           ; Load the current result into A
    ldl 0           ; Load var1 into A
    add             ; A = result + var1 (adding var1 to result)
    
    ldc res
    ldnl 0
    a2sp
    stl 0
    ldc 0
    a2sp

    ldl 1           ;
    adc -1          ; 
    stl 1           ; Store the updated value of var2

    ldl 1           ; Load var2 into A
    brz done        ; If var2 reaches 0, exit the loop
    br multiply_loop ; Otherwise, repeat the loop

done:
    HALT            ; End the program

var1:  SET 5       ; Set var1 to 10 (the first number)
var2:  SET 5        ; Set var2 to 5 (the second number)
res:   data 7      ; 