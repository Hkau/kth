puts open('nyx-head.tex').read

input = $stdin.read
input.gsub!(/\r/, '')

split = input.split(/(^h[1-4]\. .*)/)

# Get rid of stray spaces and newline characters.
level = 0
split.each do |section|
  section.strip!
  section.gsub!(/^(\*+)\s/) {|match| '>> ' * (match.size-1) }
  # replace *format* _format_ and +format+ etc.
  section.gsub!(/\"(.+?)\"/,  '``\1\'\'')
  section.gsub!(/http:\/\/([^\s]+)/, '\url{http://\1}')
  section.gsub!(/\+(.+?)\+/, '\underline{\1}')
  section.gsub!(/_(.+?)_/, '\emph{\1}')
  section.gsub!(/_/, '\_')
  section.gsub!(/(^|\s)\*(.+?)\*($|\s)/, ' \textbf{\2} ')
  section.gsub!(/\s-([^\s-].*?[^\s-])-\s/, ' \sout{\1} ')
end

split.shift
match = /^h1.\s+(.*)$/.match(split[0]) # extract document title
split.shift
split.shift
puts '\title{' + match[1] + '}'
puts
puts '\begin{document}'

# remove everything before abstract
while true do
  if split[0] =~ /^h1. abstract\s*$/i
    puts '\maketitle'
    puts
    puts '\selectlanguage{english}'
    puts '\begin{abstract}'
      print "\t", split[1]
      puts
    puts '\end{abstract}'
    puts '\selectlanguage{swedish}'
    puts
    puts '\newpage'
    puts
    puts '\tableofcontents'
    puts
    puts '\clearpage'
    puts '\setcounter{page}{1}'
    puts
    puts '\startfooter'
    puts
    split.shift
    split.shift
    break
  end
  if split[0] == nil
    $stderr.write "no abstract?! I am confused. I will crash now.\n"
    exit
  end
  split.shift
end

in_table = false
# eat the rest, nom nom :3
while not split.empty?
  #puts split[0]
  section = split[0].match /^h([1-4])\.\s+(.*)\s*$/
  if "appendix".casecmp(section[2]) == 0
    puts '\clearpage'
    puts "\t\\appendix"
    puts
    split.shift
    split.shift
    next
#  elsif section[1] == '1'
#    puts '\chapter{',section[2],'}'
  elsif section[1] == '4'
    print "\t" * 3, '\paragraph{', section[2], '}'
  else
    print "\t" * (section[1].to_i - 1), '\\', 'sub'*(section[1].to_i - 1), 'section{', section[2], '}'
  end
  level = section[1].to_i - 1
  puts
  puts
  split[1].each do |line|
    if line.strip == ''
      next
    end
    if line =~ /^\|([^|]*\|)+$/
      line.strip!
      line.slice!(0)
      line.chop!
      line = (line+' ').split '|'
      if not in_table
        in_table = true
	puts "\t" * level + '\begin{tabular} { |' + ' l |' * line.size + ' }'
	level += 1
	puts "\t" * level + '\hline'
      end
      line = (line.join ' & ') + " \\\\\n" + "\t" * level + "\\hline"
    else
      if in_table
        in_table = false
	level -= 1
	puts level * "\t" + '\end{tabular}'
      end
    end
    print "\t" * level + line
    puts
  end
  if in_table
    in_table = false
    level -= 1
    puts "\t" * level + '\end{tabular}'
  end
  puts
  split.shift
  split.shift
end

puts open('nyx-foot.tex').read

