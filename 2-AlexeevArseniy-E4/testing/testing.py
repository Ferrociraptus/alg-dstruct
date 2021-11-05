#!/bin/env python
import subprocess
from sys import argv

tests_amount = 100

if __name__ == '__main__':
	if len(argv) > 1:
		binary = argv[1]
	else:
		binary = "../lab_5"

	failed_test_counter = 0
	for i in range(tests_amount):
		test_filename_input = f"pygen_tests/input_{i}.txt"
		test_filename_output = f"pygen_tests/output_{i}.txt"
		with open(test_filename_output, "r") as output:
			ans = [line.strip() for line in output.readlines()]

		with open(test_filename_input) as input:
			process = subprocess.Popen([binary], stdin=input, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
			stdout, _ = process.communicate(timeout=2)
			stdout = stdout.decode("utf-8").splitlines()

		if stdout == ans:
			print(f"\x1b[32mTest #{i}: pass\x1b[0m")
		else:
			print(f"\x1b[31mTest #{i}: fail\x1b[0m")
			failed_test_counter+=1

	print(f"Result:")
	print(f"\x1b[31m\tFailed: {failed_test_counter}\x1b[0m")
	print(f"\x1b[32m\tPassed: {tests_amount - failed_test_counter}\x1b[0m")

