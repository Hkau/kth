#!/usr/bin/env ruby
count = {}
cryptotext = ""

text = STDIN.read
text.each_char do |c|
	if count[c].nil?
		count[c] = 0
	end
	count[c] += 1
	cryptotext += c
end

chars = []

count.each_key do |k|
	chars << k
end

chars.sort! do |a,b|
	count[a] <=> count[b]
end

chars.each do |key|
	printf "%.3f%% (%d) %s\n", (100*count[key].to_f / text.length), count[key], key
end

