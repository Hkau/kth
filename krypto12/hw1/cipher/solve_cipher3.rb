#!/usr/bin/env ruby
translation = {
	'(' => ' ', 
	'' => "\n", 
	'' => 'e', 
	'' => 't', 
	'' => 'a', 
	'' => 'o', 
	'' => 'i', 
	'' => 'n', 
	'' => 's', 
	'' => 'h', 
	'' => 'r', 
	'' => 'd', 
	'' => 'l', 
	'' => 'c', 
	'' => 'u', 
	'' => 'm', 
	'' => 'w', 
	'' => 'f', 
	'' => 'g', 
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
alphabet = "!\"#'(),-.0123456789:;?ABCDEFGHIJKLMNOPQRSTUVWYZ_abcdefghijklmnopqrstuvwxyz"
alphabet_index = {}
alphabet.length.times do |idx|
	alphabet_index[alphabet[idx, 1]] = idx
end
alphabet.each_char do |c|
	count[c] = 0
end


cryptotext = ""

rot = 0
rot_offset = 37

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

