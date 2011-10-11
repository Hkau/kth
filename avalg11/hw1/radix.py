#!/usr/bin/env python3
from collections import deque
def radix(num):
	n = len(num)

	buckets = []
	for i in range(n):
		buckets.append(deque())

	nums = deque()

	for x in num:
		nums.append((x,x))

	# This will run for m times if all numbers are < n^m
	while True:

		# O(n) (n numbers, append/popleft constant with linked list)
		# mod and integer divisions are unit cost (O(1)).
		while nums:
			(i, rem) = nums.popleft()
			idx = rem % n;
			rem //= n;
			buckets[idx].append((i, rem))

		non_nulls = 0
		# For each bucket (O(n)) + Put each number back in combined list (O(n))
		for i in range(n):
			if len(buckets[i]) > 0:
				non_nulls += 1
				nums.extend(buckets[i])
				buckets[i].clear()
	
		# If all numbers were in the same bucket, sorting is done..
		if non_nulls == 1:
			break

	# put all numbers back in the original list (O(n))
	idx = 0
	for i in range(n):
		(foo, bar) = nums.popleft()
		print(foo)
		num[i] = foo

	return num


print(radix([1234,4321,2321,234,454,6344]))

