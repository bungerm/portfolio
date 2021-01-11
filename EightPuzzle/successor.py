# Successor Function
# CSC 480 HW1 - Search
#
# Description: The Successor function for the Modified Eight Puzzle.
#      Takes a Node and exapnds it, returning an array of child nodes.
#
# See main.py for further details

from node import Node

def GetSuccessors(n):
	successors = []
	currentState = n.state
	up = True
	down = True
	left = True
	right = True

	# Check where the blank tile can move
	for i in range(0,9):
		if currentState[i] == 0:
			break
	if i < 3:
		up = False
	if i > 5:
		down = False
	if i == 0 or i == 3 or i == 6:
		left = False
	if i == 2 or i == 5 or i == 8:
		right = False
	
	# Create the child nodes based of where the blank tile can move
	if up:
		newState = currentState[:]
		newState[i] = newState[i-3]
		newState[i-3] = 0
		successors.append(Node(newState, n, "UP", newState[i]))
	if left:
		newState = currentState[:]
		newState[i] = newState[i-1]
		newState[i-1] = 0
		successors.append(Node(newState, n, "LEFT", newState[i]))
	if down:
		newState = currentState[:]
		newState[i] = newState[i+3]
		newState[i+3] = 0
		successors.append(Node(newState, n, "DOWN", newState[i]))
	if right:
		newState = currentState[:]
		newState[i] = newState[i+1]
		newState[i+1] = 0
		successors.append(Node(newState, n, "RIGHT", newState[i]))

	return successors