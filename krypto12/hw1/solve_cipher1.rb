#!/usr/bin/env ruby
count = {}
cryptotext = ""
translation = {
	'W' => " ",
	'U' => 't',
	'P' => 'e',
	'M' => 'a',
	'R' => 'o',
	'T' => 'i',
	'_' => 'n',
	'A' => 's',
	'G'=> 'h',
	'Z'=> 'r',
	'#' => 'd',
	'L' => 'l',
	'K' => 'c',
	'Y' => 'u',
	'O' => 'm',
	'I' => 'w',
	'D' => 'f',
	'B' => 'g',
	'S' => 'y',
	'V' => 'p',
	'E' => 'b',
	'C' => 'v',
	'H' => 'k',
	'F' => 'j',
	'X' => 'x',
	'Q' => 'q',
	'N' => 'z',
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
	STDERR.print translation[c]
end
puts

count.each_key do |key|
	print (100*count[key].to_f / cryptotext.length).to_s[0,5] + "% (" + count[key].to_s + ") " + key
	if translation[key].nil?
		puts " *"
	else
		puts " => " + translation[key]
	end
end

