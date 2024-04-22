#!/usr/bin/env simplang

begin_loop
    get_option fact "Enter number to calculate the factorial of: "
    if_less_or_equal fact 0
        assign fact 0
        print "Number must be a positive number greater than 0"
        continue
    end_if
end_loop_if_equal 0 0

assign iter (subtract fact 1)
begin_loop
    multiply fact iter
    decrement iter
end_loop_if_less_or_equal iter 1

if_equal fact 0
   assign fact 1
end_if

print fact

done
