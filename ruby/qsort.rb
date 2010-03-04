def qsort(v)
	qsort_int(v, 0, v.length-1)
end

def qsort_int(v, idx_start, idx_end)
	if idx_start >= idx_end
		return
	end

	#isort här :)

	piv_idx = idx_start + rand( idx_end - idx_start + 1)
	piv = v[ piv_idx ]

	v[ piv_idx ] = v[ idx_start ]

	idx_lower = idx_start+1
	idx_upper = idx_end

	while idx_lower <= idx_upper
		if v[idx_lower] >= piv
			tmp = v[idx_upper]
			v[idx_upper] = v[idx_lower]
			v[idx_lower] = tmp
			idx_upper -= 1
		else
			idx_lower += 1
		end
	end

	idx_lower -= 1
	v[ idx_start ] = v[ idx_lower ]
	v[ idx_lower ] = piv

	qsort_int(v, idx_start, idx_lower - 1)
	qsort_int(v, idx_lower + 1, idx_end)
end

