#!/usr/bin/env simplang

; Set our initial values and string literals
assign guess 0
assign lower_bound 0
assign upper_bound 101
assign counter 0
assign invalid_target "Now I'm not cheating or anything, but I know that's out of range!"
assign ill_guess "I'll guess "
assign too_low ", and that's too low"
assign too_high ", and that's too high"

print "Enter a number between 1 and 100 and I'll try to guess it!"

begin_loop
    print_n "Enter your number here: "
    get_input target

    ; If the user enters a number outside the acceptable range,
    ; display a message stating so and request a new number
    if_greater target 100
        print invalid_target
        assign target 0
        continue
    end_if
    if_less target 1
        print invalid_target
        assign target 0
        continue
    end_if

; Always end the loop if we reach this point, as that means that
; the entered number is within range
end_loop_if_equal 0 0

begin_loop
    print_n ill_guess
    ; Resets guess to 0
    subtract guess guess
    add guess (add lower_bound upper_bound)
    divide guess 2
    print_n guess

    ; The guess is the midpoint between the upper and lower bounds,
    ; rounded down. Here we update either the upper or lower bound
    ; based on whether the guess was higher or lower than the answer
    if_less guess target
        print too_low
        assign lower_bound guess
    end_if
    if_greater guess target
        print too_high
        assign upper_bound guess
    end_if

    increment counter
end_loop_if_equal guess target

; Prints the success message
print ", and that's it!"
print_n "It took me "
print_n counter
print " guesses"

done