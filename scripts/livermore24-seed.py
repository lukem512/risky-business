#!/bin/python

# Input should contain a random number per line
lines = [line.rstrip('\n') for line in open('random.txt')]
base = 100
n = 0
numbers = []
m = 0
print "%%"
for line in lines:
	print "LDC r4 " + line
	# base = r10
	print "ST r4 " + str(base + n)
	numbers.append(int(line))
	if numbers[n] < numbers[m]:
		m = n
	n += 1
print "%%"
print "% minimum value is " + str(numbers[m]) + " at m = " + str(m)