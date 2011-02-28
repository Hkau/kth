puts open('nyx-head.tex').read

input = $stdin.read
input.gsub!(/\r/, '') # removes \r from possible \r\n
input.gsub!(/\$TOC\$/, '\clearpage \tableofcontents \clearpage')
input.gsub!(/\$VERSIONS\$/) {|match| open('versions.txt').read }
input.gsub!(/\$ISSUES\$/) {|match| open('issues.txt').read }
input.gsub!(/!([^\s!]+)!/){ |s|
  name = $1
  name.gsub!('å', 'aa')
  name.gsub!('ä', 'ae')
  name.gsub!('ö', 'oe')

  '\begin{figure}[ht] \centering \includegraphics[width=0.8\textwidth]{' + name + '} \end{figure} \FloatBarrier'
}

num = input[/[0-9]\.[0-9]/].to_f

if num < 1.0
  version = ''
elsif num == 1.0
  version = ' -- \emph{Final Draft}'
else
  version = ' -- \emph{Revised}'
end

puts '\version{' + num.to_s + version + '}'

split = input.split(/(^h[1-5]\. .*)/)

# Get rid of stray spaces and newline characters.
level = 0
defaultformat = 'p{2.6cm} p{12.5cm}'
tableformat = defaultformat

hidden_sect = ['Ändringslogg', 'Dokumentversioner', 'Gruppmedlemmar']

split.each do |section|
  section.strip!
  section.gsub! /&/, '\\\&'
  tmp = section.split /\n/
  tmp.each do |line|
    if line =~ /^\\begin\{figure\}\[ht\]/ # don't touch if it's a figure
      next
    end
    line.gsub!(/^(\*+)\s/) {|match| '>> ' * (match.size-1) }
  # replace *format* _format_ and +format+ etc.
    line.gsub!(/\"(.+?)\"/,  '``\1\'\'')
    line.gsub!(/http:\/\/([^\s]+)/, '\url{http://\1}')
#    line.gsub!(/\+(^_+?)\+/, '\underline{\1}')
    line.gsub!(/(^|\W)(\*_|_\*)([^\|]+?)(\*_|_\*)(\W|$)/, '\1\textbf{\emph{\3}}\5')
    line.gsub!(/(^|\W)_([^\|]+?)_(\W|$)/, '\1\emph{\2}\3')
    line.gsub!(/(^|\W)\*(.+?)\*(\W|$)/, '\1\textbf{\2}\3')
  #section.gsub!(/^_([^\|]+?)_$/, '\emph{\1}')
    line.gsub!(/_/, '\_')
    line.gsub!(/#/, '\#')
    line.gsub!(/%/, '\%')
    line.gsub!(/\\url\{([^\}]*)\}/) { |s|
      url = $1
      url.gsub!('\_', '_') # \_ => _ inside urls.
      url.gsub!('\#', '_') # \# => # inside urls.
      '\url{' + url + '}'
    }
    line.gsub!(/\s-([^\s-].*?[^\s-])-\s/, ' \sout{\1} ')
    line.gsub!(/\^/, '\^')
    line.gsub!(/(\d+)\\\^(\d+)/, '$\1^{\2}$')
  end
  section[0..-1] = tmp.join "\n"
  #section.gsub! /^(\s*$)+/m, "\n\n"
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
    puts '\clearpage'
    puts '\thispagestyle{empty}'
    puts '\mbox{}'
    puts '\newpage'
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
#    puts '\tableofcontents'
#    puts
#    puts '\clearpage'
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
  section = split[0].match /^h([1-5])\.\s+(.*)\s*$/
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
    print "\t" * 3, '\paragraph{', section[2], '}\\'
  elsif section[1] == '5'
    print "\t" * 3, '\subparagraph{\emph{', section[2], '}}\\'
  else
    if section[1] == '1'
      puts '\clearpage'
    end
    print "\t" * (section[1].to_i - 1), '\\', 'sub'*(section[1].to_i - 1), 'section', hidden_sect.index(section[2]) != nil ? '*' : '', '{', section[2], '}'
    puts
  end
  level = section[1].to_i - 1
  puts
  puts
  split[1].each do |line|
    if line =~ /^\|([^|]*\|)+$/
      line.strip!
      line.slice!(0)
      line.chop!
      line = (line+' ').split '|'
      if not in_table
        in_table = true
	puts "\t" * level + '\begin {table} [ht] \begin{tabular} { ' + tableformat + ' }'
	#puts "\t" * level + '\begin{tabular} { |' + ' l |' * line.size + ' }'
	level += 1
	puts "\t" * level + '\hline'
      end
      cols = []
      line.each do |word|
        if word[0..2] == '\_.'
	  cols << '\sffamily\textbf{' + word[3..-1] + '}'
	else
          cols << word
	end
      end
      line = (cols.join ' & ') + " \\\\\n" + "\t" * level + "\\hline"
    else
      if in_table
        in_table = false
	level -= 1
	puts "\t" * level + '\end{tabular} \end{table} \FloatBarrier'
	puts "\t" * level + '\vspace{6mm}'
	puts
      end
    end
    line.gsub!(/\$TABLEFORMAT:(.*)\$/) { |match|
      if $1 == 'DEFAULT'
        tableformat = format
      else
        tableformat = $1
      end
      ''
    }
    if line.strip == ''
      next
    end
    print "\t" * level + line
    puts
  end
  if in_table
    in_table = false
    level -= 1
    puts "\t" * level + '\end{tabular} \end{table} \FloatBarrier'
    puts
  end
  puts
  split.shift
  split.shift
end

puts open('nyx-foot.tex').read

