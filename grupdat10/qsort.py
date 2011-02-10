import random
def qsort(v):
	quicksort(v, 0, len(v)-1)


def quicksort(v, left, right):
	# return if there are no more elements to be sorted
	if left >= right:
		return

	# insertion sort here

	pivot_idx = random.randint(left, right) # pick a random pivot
	# let 'em know why..

	#put it first while partitioning the list
	pivot = v[pivot_idx]
	v[pivot_idx] = v[left]
	v[left] = pivot

	low_idx = left+1
	high_idx = right

	while low_idx <= high_idx:
		if v[low_idx] >= pivot:
			# found a 'high' number, place it last
			tmp = v[high_idx]
			v[high_idx] = v[low_idx]
			v[low_idx] = tmp
			# know one more number at the right side is correct
			high_idx -= 1
		else:
			# if not, we know the left one is correct
			low_idx += 1

	low_idx -= 1

	# place pivot back in middle
	v[left] = v[low_idx]
	v[low_idx] = pivot

	quicksort(v, left, low_idx - 1) # left partition, excluding pivot
	quicksort(v, low_idx + 1, right) # right partition, excluding pivot

arr = []
for i in range(100):
	arr.append(random.randint(0,100))

print arr
print '==>'
a2 = arr
a2.sort()
print a2
print '=?='
qsort(arr)
print arr

