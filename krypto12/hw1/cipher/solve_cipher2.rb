#!/usr/bin/env ruby
translation = {
	'_' => ' ', 
	'' => "\n", 
	'F' => 'e', 
	'T' => 't', 
	'A' => 'a', 
	'P' => 'o', 
	'#' => 'i', 
	'N' => 'n', 
	'S' => 's', 
	'' => 'h', 
	'R' => 'r', 
	'D' => 'd', 
	'L' => 'l', 
	'' => 'c', 
	'U' => 'u', 
	'M' => 'm', 
	'' => 'w', 
	'' => 'f', 
	'H' => 'g', 
	'' => 'y', 
	'' => 'p', 
	'' => 'b', 
	'' => 'v', 
	'' => 'k', 
	'' => 'j', 
	'' => 'x', 
	'' => 'q', 
	'' => 'z', 
}
count = {}
alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
alphabet_index = {}
alphabet.length.times do |idx|
	alphabet_index[alphabet[idx, 1]] = idx
end

cryptotext = ""

alphabet.each_char do |c|
	count[c] = 0
end

rot = 0
rot_offset = 20

STDIN.each_char do |c|
	if c == "\n"
		next
	end
	cryptotext += alphabet[(alphabet_index[c] + rot) % alphabet.length, 1]
	rot += rot_offset
end

cryptotext.each_char do |c|
	count[c] += 1
end

cryptotext.each_char do |c|
	if translation[c].nil?
		print c
		next
	end
	print translation[c]
end
puts

count.each_key do |key|
	STDERR.printf "%.3f%% (%d) %s", (100*count[key].to_f / cryptotext.length), count[key], key
	if translation[key].nil?
		STDERR.puts " *"
	else
		STDERR.puts " => " + translation[key]
	end
end

