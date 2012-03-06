#!/usr/bin/env ruby
words = {}
STDIN.read.split(" ").each do |foo|
	words[foo] = foo
end

word_list = []
words.each_key do |k|
	word_list << k
end

word_list.sort! do |a,b|
	if a.length == b.length
		a <=>b
	else
		a.length <=> b.length
	end
end

word_list.each do |w|
	puts w
end
