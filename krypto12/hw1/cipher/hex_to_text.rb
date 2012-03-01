#!/usr/bin/env ruby

alphabet = "!\"#'(),-.0123456789:;?ABCDEFGHIJKLMNOPQRSTUVWYZ_abcdefghijklmnopqrstuvwxyz"
alpha_index = 0

translation = {}

foo = ""
i = 0
STDIN.each_char do |c|
	if c == "\n"
		i += 1
		if i == 2:
			i = 0
			puts
		end
		next
	end
	foo += c
	if foo.length == 1
		next
	end
	c = foo
	if translation[c].nil?
		translation[c] = alphabet[alpha_index, 1]
		alpha_index += 1
	end
	print translation[c]
	foo = ""
end

puts translation.length
