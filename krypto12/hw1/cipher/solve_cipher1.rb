#!/usr/bin/env ruby
count = {}
cryptotext = ""
translation = {
	'W' => ' ', #
	'D' => "\n", #
	'U' => 'e', #
	'P' => 't', #
	'M' => 'a', #
	'_' => 'o', #
	'A' => 'i', #
	'R' => 'n', #
	'G' => 's', #
	'T' => 'h', #
	'Z' => 'r', #
	'L' => 'd', #
	'#' => 'l', #
	'C' => 'c', #
	'Y' => 'u', #
	'I' => 'm', #
	'O' => 'w', #
	'B' => 'f', #
	'K' => 'g', #
	'E' => 'y', #
	'H' => 'p', #
	'V' => 'b', #
	'F' => 'v', #
	'S' => 'k', #
	'J' => 'j', # -- not found --
	'X' => 'x', #
	'Q' => 'q', #
	'N' => 'z', #
}

"#ABCDEFGHIJKLMNOPQRSTUVWXYZ_".each_char do |c|
	count[c] = 0
end

STDIN.each_char do |c|
	if c == "\n"
		next
	end
	count[c] += 1
	cryptotext += c
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

