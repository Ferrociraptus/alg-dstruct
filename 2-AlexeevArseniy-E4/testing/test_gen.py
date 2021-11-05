#!/bin/env python
from random import randint, choice

if __name__ == '__main__':
	tests_amount = 100
	max_operations_amount_in_test = 25
	value_gen = lambda: randint(0, 10000)
	
	for i in range(tests_amount):
		with open(f"pygen_tests/input_{i}.txt", "w") as input:
			with open(f"pygen_tests/output_{i}.txt", "w") as output:
				checker = set()
				
				# lines in test
				for k in range(max_operations_amount_in_test):
					operation = randint(0, 2)

					# add to binary tree operation
					if operation == 0:
						val = value_gen()
						checker.add(val)
						input.write(f"a {val}\n")
						
					# remove
					elif operation == 1:
						if len(checker) > 0:
							val = choice(tuple(checker))
							checker.remove(val)
						else:
							val = value_gen()
						input.write(f"r {val}\n")
						
					# find
					elif operation == 2:
						if randint(0, 1) and len(checker) > 0:
							val = choice(tuple(checker))
						else:
							val = value_gen()
						
						input.write(f"f {val}\n")
						
						if val in checker:
							output.write("yes\n")
						else:
							output.write("no\n")
