puts open('nyx-head.tex').read

input = $stdin.read

split = input.split(/(^h[1-4]\. .*)/)

# Get rid of stray spaces and newline characters.
split.each do |section|
  section.strip!
  # replace *format* _format_ and +format+
  section.gsub!(/^(\*+)\s/) {|match| '>> ' * (match.size-1) }
  section.gsub!(/\"(.+?)\"/,  '``\1\'\'')
  section.gsub!(/\[http:\/\/(.+?)\]/, '\url{http://\1}')
  section.gsub!(/\+(.+?)\+/, '\underline{\1}')
  section.gsub!(/_(.+?)_/, '\emph{\1}')
  section.gsub!(/\*(.+?)\*/, '\textbf{\1}')
end

# remove everything before abstract
while true do
  if split[0] =~ /^h1. abstract\s*$/
    puts '\maketitle'
    puts
    puts '\begin{abstract}'
      print "\t", split[1]
      puts
    puts '\end{abstract}'
    puts
    puts '\newpage'
    puts
    puts '\tableofcontents'
    puts
    puts '\newpage'
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

# eat the rest, nom nom :3
while not split.empty?
  #puts split[0]
  section = split[0].match /^h([1-4])\.\s+(.*)\s*$/
  if section[2] == "appendix"
    puts "\t\\appendix"
    puts
    split.shift
    split.shift
    next
  elsif section[1] == '1'
    print '\chapter{',section[2],'}'
  else
    print "\t" * (section[1].to_i - 1), '\\', 'sub'*(section[1].to_i - 2), 'section{', section[2], '}'
  end
  puts
  puts
  split[1].each do |line|
    print "\t" * (section[1].to_i), line
    puts
  end
  puts
  split.shift
  split.shift
end

puts open('nyx-foot.tex').read

