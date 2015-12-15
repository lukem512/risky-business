#!/bin/python

# Input should contain a seed cell, one-per-line, in the form x,y,value
lines = [line.rstrip('\n') for line in open('scrubber.gol')]
print "%%"
for line in lines:
	tokens = line.split(',')	
	print "LDC r7 " + tokens[1]
	# r5 = by
	print "ADD r6 r5 r7"
	# r0 = width
	print "MUL r6 r0 r6"
	print "LDC r7 " + tokens[0]
	# r4 = bx
	print "ADD r7 r4 r7"
	print "ADD r6 r2 r6"
	print "ADD r7 r6 r7"
	print "LDC r6 " + tokens[2]
	print "STR r7 r6"
	print "%%"