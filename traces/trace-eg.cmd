# Demonstration of queue testing framework
# Use help command to see list of commands and options
# Initial queue is NULL.
show
# Create empty queue
new
# See how long it is
size
# Fill it with some values. First at the head
ih dolphin
ih bear
ih gerbil
ih aaa
# Now at the tail
it meerkat
it bear
it bbb
it bbb
it ccc
it ccc
it bbb
# remove head
rh
# remove tail
rt bbb
# delete mid
dm
# delete dup
dedup
# Reverse it
reverse
# insert a, b, c
it a
it b
it c
# ReverseK k = 2
reverseK 2
reverseK 2
# ReverseK k = 3
reverseK 3
reverseK 3
# ReverseK k = 7
reverseK 7
reverseK 7
# swap
swap
swap
# sort
sort
# See how long it is
size
# Delete queue. Goes back to a NULL queue.
free
# Exit program
quit
