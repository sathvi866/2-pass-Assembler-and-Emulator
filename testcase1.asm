    adj 0
    ldc var1
    stl 0

    ldl 0          ; Reload the value of var1 (5) into A
    brz zero
    brlz negative

positive:
    ldc 1
    stl 2
    br exit

negative:
    ldc 0
    stl 2
    br exit

zero:
    ldc 2
    stl 2

exit:
    HALT

var1: SET 5