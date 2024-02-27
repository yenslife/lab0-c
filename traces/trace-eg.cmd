# Demonstration of queue testing framework
# Use help command to see list of commands and options
# Initial queue is NULL.
show
# Create empty queue
new
# See how long it is
size
# Fill it with some values.  First at the head
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
# See how long it is
size
# Delete queue.  Goes back to a NULL queue.
free
# Exit program
quit
