    adj 0           ; Initialize SP to 0
    ldc var1        ; Load var1 (4) into A
    stl 0           ; Store var1 (4) in memory location 0

    ldc var1        ; Load var1 (4) again into A (for multiplying 4 * 4)
    stl 1           ; Store var1 (4) in memory location 1

    ldc 0           ; Load 0 into A (initial result is 0)
    stl 2           ; Store the initial result of 4^2 in memory location 2

    ldl 1           ; Load var1 (4) into A (number of times to add)
    brz x2_done     ; If var1 is 0, exit (4^2 is 0)

multiply_loop:
    ldl 2           ; Load the current result into A
    ldl 0           ; Load var1 (4) into A
    add             ; A = result + 4 (adding 4 to result)
    stl 2           ; Store the updated result in memory location 2

    ldl 1           ; Load var1 (4) into A
    adc -1          ; Decrement the value of var1 by 1
    stl 1           ; Store the updated value of var1 (decremented)

    ldl 1           ; Load var1 into A
    brz x2_done     ; If var1 reaches 0, exit the loop
    br multiply_loop ; Otherwise, repeat the loop

x2_done:
    ; Now we have 4^2 in memory location 2, we will multiply it by 4 to get 4^3

    ldc var1        ; Load var1 (4) into A
    stl 3           ; Store var1 (4) in memory location 3 for 4^3 multiplication

    ldc 0           ; Load 0 into A (initial result for 4^3)
    stl 4           ; Store the initial result of 4^3 in memory location 4

    ldl 3           ; Load var1 (4) into A (number of times to add)
    brz x3_done     ; If var1 is 0, exit (4^3 is 0)

x3_multiply_loop:
    ldl 4           ; Load the current result into A
    ldl 2           ; Load 4^2 into A
    add             ; A = result + 4^2 (adding 4^2 to result)
    stl 4           ; Store the updated result in memory location 4

    ldl 3           ; Load var1 (4) into A
    adc -1          ; Decrement the value of var1 by 1
    stl 3           ; Store the updated value of var1 (decremented)

    ldl 3           ; Load var1 (4) into A
    brz x3_done     ; If var1 reaches 0, exit the loop
    br x3_multiply_loop ; Otherwise, repeat the loop

x3_done:
    HALT            ; End the program

var1:  SET 4        ; Set var1 to 4 (the input number, 4)
res:   data 0       ; Store the result of 4^3
