# Node Class
# CSC 480 HW1 - Search
#
# Description: This is a node data structure used in search algorithms
#      for the Modified Eight Puzzle
#
# See main.py for further details
#
# Each nodes contains:
# State: the current state of the system (Array of int values)
#      Example: {1,2,3,8,0,4,6,5}
# Action: the action taken to reach this node
#      Example: up,down,left,right,root(starting node)
# Depth: depth of the node in the tree
# Cost: cost of the action taken (For the Modified Eight Puzzle, the value of the moved tile)
# Path cost: cost of path to reach this node
#      Example: modified eight puzzle uses value moved as path weight
# Parent: the parent of this node
# Expanded: boolean to show if node as been expanded
# Children: if it has been expanded, what are it's chidlren

from random import randint

class Node:
	# constructor for Node
	def __init__(self, state, parent=None, action=None, cost=0):

		self.state = state
		if parent==None:
			self.action = None
			self.parent = None
			self.depth = 0
			self.cost = 0
			self.pathCost = 0
		else:
			self.action = action
			self.parent = parent
			self.depth = parent.depth + 1
			self.cost = cost
			self.pathCost = parent.pathCost + cost

	# If there is a tie in the min heap, randomly assign one node the winner
	def __lt__(self, other):
		if randint(0,1) == 0:
			return self
		else:
			return other
		