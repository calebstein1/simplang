#!/usr/bin/env simplang

; Get the two numbers to operate on from the user
; They can be passed in as command line arguments,
; in which case we can skip prompting for the number
get_option num1 "Enter first number: "
get_option num2 "Enter second number: "

; Copy the initial values that we can print them at the end
assign orig_num1 num1
assign orig_num2 num2

; Calculate the GCD using Euclid's algorithm
begin_loop
    assign temp num2
    assign num2 (modulo num1 num2)
    assign num1 temp
end_loop_if_equal num2 0

; Form and print the final output
print_n "The GCD of "
print_n orig_num1
print_n " and "
print_n orig_num2
print_n " is "
print num1

done