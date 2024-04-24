#!/usr/bin/env simplang

begin_loop
    get_option num_count "Enter number of items to sort: "
    if_less_or_equal num_count 1
        print "Number must be greater than 1"
        assign num_count 0
        continue
    end_if
end_loop_if_equal 0 0

; Initialize and populate number array
new_array nums num_count
for_each num in nums
    random num 1000
end_loop

print "Unsorted:"
for_each num in nums
    print_n num
    print_n " "
end_loop
print ""

assign counter num_count

begin_loop
    for_each num in nums
        if_greater num next
            swap num next
        end_if
    end_loop
    decrement counter
end_loop_if_equal counter 0

print "Sorted:"
for_each num in nums
    print_n num
    print_n " "
end_loop
print ""

done