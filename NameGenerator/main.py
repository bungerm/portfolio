# CSC 480
# HW 4 - Baby Name Generator
#
# Populates a Markov Model with probabilities from a corpus of names
# Then generates random name from the model
#
# User Inputs:
# Male or female
# Minimum name length
# Maximum name length
# Order of the model
# Number of names to generate

from itertools import product
from random import randrange


# Generates random names based on the provided hidden markov model,
# minimum name length, maximum name length, amount of names requested,
# and the order given.
def generateNames(model, m, M, amount, order):
	names = []
	count = 0
	# until there are the right amount of names
	while count < amount:
		# create the appropriate prefix (characters to generate the first letters by)
		prefix = "-"*order
		name = prefix
		char = ""
		# until the character chosen is not the ending character "-"
		while char != "-":
			# update the prefix with each added character
			prefix = name[-order:]
			#create a random number between 0 and 1
			number = randrange(1001)/1000
			# populate a list with the possible next characters and their probabilities
			potential = []
			for key in model:
				if key[0] == prefix:
					potential.append((key[1], model[key]))
			# sort the potentials list by their probabilities
			potential.sort(key = lambda x: x[1], reverse = True)
			# pick a character based on the random number
			for i in range(len(potential)):
				if potential[i][1] >= number:
					char = potential[i][0]
					name = name + char
					break
				else:
					number = number - potential[i][1]
		# check if new name meets the requirements
		# (min and max length, already existsing in corpus)
		name = name[order:-1]
		name = name[0].upper() + name[1:]
		if (len(name) >= m and len(name) <= M):
			if name not in model.keys():
				names.append(name)
				count = count + 1
	return names

# Generates the Hidden Markov Model from a given corpus and order
def generateModel(corpus, order):
	model = {}
	# store all the possible characters for the names
	alphabet = "a b c d e f g h i j k l m n o p q r s t u v w x y z -".split()
	prod = []
	# create the cartesian product of every possible character to the degree of the given order: "a, b, c..."^order
	# thats 26^order, subtracting the 27th character meant ot end the name
	for i in range(0, order+1):
		prefix = "-"*i
		prod = product(alphabet[:-1], repeat=(order-i))
		for j in prod:
			for letter in alphabet:
				#  the key = the created sequences with every possible following letter as a tuple
			    key = (prefix + "".join(j), letter)
			    # add the key and its defautl value to the model dictionary
			    model[key] = 0.0
	# go through the corpus and add to the counts in the model
	# e.g. --john- with order = 2 would first add one count of ("--", "j")
	with open(corpus) as file:
		for line in file:
			name = "-"*order + line.rstrip('\n').lower() + "-"
			# for every name "--jim-"
			for i in range(order, len(name)):
				prefix = ""
				# for the preceding characters for each character in name
				for j in range(i-order, i):
					prefix = prefix + name[j]
				# add to the counter for the character and its prefix in the model dictionary
				model[(prefix, name[i])] = model[(prefix, name[i])] + 1
	
	# Here i go through the possible combinations twice
	# The first calculates the sum of the characters associated with a given combination
	#	- create an array and store the sums in the order calculated
	#	- start new count every 27 characters (length of alphabet, aka the possible characters)
	# The second then divides each character's value by the appropriate sum
	#	- iterates through counts array every 27 characters, deviving all values appropriately
	counts = []
	total = 0
	index = 0
	a = len(alphabet)

	for key in model:
		index = index + 1
		total = total + model[key]

		if index >= a:
			index = 0
			counts.append(total)
			total = 0

	prefixIndex = 0
	for key in model:
		index = index + 1
		if (counts[prefixIndex] != 0.0):
			model[key] = model[key]/counts[prefixIndex]
		if index >= a:
			prefixIndex = prefixIndex + 1
			index = 0

	return model

def main():
	fileStrings = ["namesBoys.txt", "namesGirls.txt"]
	outputStrings = ["male or female: ", "min name length: ", "max name length: ", "order of model: ", "number of names: "]
	inputString = []

	for output in outputStrings:
		inputString.append(input(output))
	if inputString[0] == "male":
		gender = 0
	else:
		gender = 1
	minLength = int(inputString[1])
	maxLength = int(inputString[2])
	order = int(inputString[3])
	amount = int(inputString[4])

	model = generateModel(fileStrings[gender], order)
	names = generateNames(model, minLength, maxLength, amount, order)
	print(names)

main()