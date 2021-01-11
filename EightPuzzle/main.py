# Main Class
# CSC 480 HW1 - Search
# 
# Description: This is project A1 of Artificial Intelligence I - Winter 2019
#
#########################
# MODIFIED EIGHT PUZZLE #
#########################
#
# Multiple search types are implemented to find solutions for the Modified Eight Puzzle:
# BreadthFirst, DepthFirst, Iterative Deepening, Uniform Cost, BestFirst, and AStar with 3 different heuristics
#
# Usage: Search (startSTate, goalState, searchType)
# * Iterative Deepening is it's own function, do not use it as searchType
#
# Board state is a list of 9 numbers: [1,2,3,4,0,5,6,7,8]

from heapq import *
from node import Node
from successor import GetSuccessors

# Displays the solution
# This function is given final node of a solution, goes up the chain of parents,
# and adds them to a Python list. The list is then printed out using DisplayNode
def DisplaySolution(node, goalState, maxQueueSize, lenGraveyard):
	n = node
	solution = []
	solution.append(n)
	while n.parent != None:
		n = n.parent
		solution.append(n)
	while solution:
		DisplayNode(solution.pop())
	print("Max Queue Size: " + str(maxQueueSize))
	print("Nodes Dequeued: " + str(lenGraveyard))
	print("Goal State Reached")

def DisplayFailure(maxQueueSize, lenGraveyard):
	print("Max Queue Size: " + str(maxQueueSize))
	print("Nodes Dequeued: " + str(lenGraveyard))
	print("No Solution Found")

# Displays important information regarding a given node
#    - The action taken to get to this node (left, down, right up)
#    - The cost of path to this node (in modified eight puzzle: value of tile moved)
#    - The path cost or total cost to reach this node
#    - The depth of the node within the tree
#    - The board state
def DisplayNode(node):
	print(str(node.action) + ", cost = " + str(node.cost) + ", path cost = " + str(node.pathCost) + ", depth = " + str(node.depth))
	print(node.state)

# Counts how many tiles are not in the correct spot
# For every tile in the surrent state, compare to goal state
def MisplacedTiles(currentState, goalState):
	h1 = 0
	for tile in range(0, len(currentState)):
		if currentState[tile] != goalState[tile]:
			h1 += 1
	return h1

# Calculates the Manhattan Distance
# Manhattan Distance is the sum of the distances each tile must travel to reach goal position
def ManhattanDistance(currentState, goalState):
	h2 = 0
	for i in range(0,9):
		c = currentState.index(i)
		g = goalState.index(i)
		h2 += abs(c%3 - g%3) + abs(c//3 - g//3)
	return h2

# Manhattan Distance 2 is my my custom heuristic h3
# Multiplies the x and y distances by the value of the tile being moved
def ManhattanDistance2(currentState, goalState):
	h3 = 0
	for i in range(0,9):
		c = currentState.index(i)
		g = goalState.index(i)
		h3 += abs(c%3 - g%3)*i + abs(c//3 - g//3)*i
	return h3

#############################
# Start of the search types #
#############################
# For cost based search strategies, a min heap is used for the queue with a calculated priority value for a popped node.
# The priority value is based on the search strategy, then fed into the Search function via the parameter searchType.

# BreadthFirst: FIFO
def BreadthFirst(node, goalState, p):
	priority = p
	priority += 1
	return priority

# DepthFirst: LIFO
def DepthFirst(node, goalState, p):
	priority = p
	priority -= 1
	return priority

# Iterative Deepening is NOT USED AS A PARAMETER OF SEARCH
# Calls Search(,, BreadthFirst, k): k being iterated
def IterativeDeepening(goalState, startState):
	i = 0
	while Search(startState, goalState, DepthFirst, i) != True:
		i += 1

# Uniform Cost: f(n) = g(n); g(n) = path cost
def UniformCost(node, goalState, p=0):
	return node.pathCost

# BestFirst: f(n) = h(n); h(n) = Misplaced Tiles
def BestFirst(node, goalState, p=0):
	return MisplacedTiles(node.state, goalState)

# AStar1: f(n) = g(n) + h(n); g(n) = path cost, h(n) = Misplaced Tiles
def AStar1(node, goalState, p=0):
	return MisplacedTiles(node.state, goalState) + node.pathCost

# AStar2: f(n) = g(n) + h(n); g(n) = path cost, h(n) = Manhattan Distance
def AStar2(node, goalState, p=0):
	return ManhattanDistance(node.state, goalState) + node.pathCost

# AStar3: f(n) = g(n) + h(n); g(n) = path cost, h(n) = Manhattan Distance2
def AStar3(node, goalState, p=0):
	return ManhattanDistance2(node.state, goalState) + node.pathCost

# Search uses a given cost based search strategy with the searchType parameter and uses it with
# a min heap to pop the least priority node.
def Search(startState, goalState, searchType, k=-1):
	# The root node is created
	root = Node(startState)

	# The queue is defined (will be min heap)
	q = []
	
	# A copy of the state of the queue nodes
	# Python sets are hash tables, so this gives
	# constant time lookups for repeat states
	qSet = set([])

	# Priority is the value given to a node based on the
	# search strategy. Priority is used for the min heap.
	priority = 0
	priority = searchType(root, goalState, priority)

	# Put the root on the min heap
	heappush(q, (priority, root))

	# Add the initial state to the queue set
	qSet.add(''.join(map(str, root.state)))

	# Graveyard holds all the popped node board states
	# Another set for constant time lookup
	graveyard = set([])
	
	# Keeps track of the max queue size
	maxQueueSize = 0

	# While the queue has anything in it
	while q:

		# Keep track of the maximum queue size
		if len(q) > maxQueueSize:
			maxQueueSize = len(q)

		# POP NODE OFF QUEUE (min heap)
		nextNode = heappop(q)[1]

		# Iterative Deepening check: if depth is beyond k then dont continue (do continue) down this path
		if k >= 0 and nextNode.depth > k:
			continue

		# Make the board state into a string for the sets
		# Remove the node's state from the queue set
		# Add the node's state to the graveyard set
		nodeString = ''.join(map(str, nextNode.state))
		qSet.remove(nodeString)
		graveyard.add(nodeString)

		# Check if popped node reached goal state
		if nextNode.state == goalState:
			DisplaySolution(nextNode, goalState, maxQueueSize, len(graveyard))
			return True

		# Otherwise, expand the node
		else:
			successors = GetSuccessors(nextNode)

			# If the board state of the child node is equal to that of state in
			# the graveyard or the queue, then don't use it
			for child in successors:
				childString = ''.join(map(str, child.state))
				if childString not in graveyard and childString not in qSet:

					# Calculate new priority, push child onto heap, and add state to queue set
					priority = searchType(child, goalState, priority)
					heappush(q, (priority, child))
					qSet.add(childString)

	# If you reached here, you HAVE FAILED
	DisplayFailure(maxQueueSize, len(graveyard))
	return False

def main():
	# The initial board state (use as parameter for search)
	easyState = [1,3,4,8,6,2,7,0,5]
	mediumState = [2,8,1,0,4,3,7,6,5]
	hardState = [5,6,7,4,0,8,3,2,1]
	# The final board state we are trying to reach (use as parameter for search)
	goalState = [1,2,3,8,0,4,7,6,5]

	# Parameters for Search: initial state, goal state, search type
	Search(hardState, goalState, BestFirst)
	
	# Iterative Deepening has it's own method (easier this way)
	#IterativeDeepening(hardState, goalState)

main()