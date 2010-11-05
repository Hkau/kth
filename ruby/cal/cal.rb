# TimeEdit iCal fancifyer. Written by
# Peter Boström <pbos@kth.se> , 2010.

$course_names = { # Any comma you may want needs to be escaped '\,'
	"DD1364" => "Databas",
	"DD1365" => "MVK",
	"DD2380" => "AI",
	"DD2387" => "C++",
	"DS1385" => "Jap FK2",
	"SF1901" => "Sannstat",
	"SF1904" => "Markov",
}

def is_course_code (line)
  return (line.size == 6 and line =~ /[A-Z0-9][A-Z0-9][0-3][0-9][0-9][0-9XVUAN]/)
end

$event_types = {
	"frl"=>"Frl",
	"ten"=>"TEN",
	"ovn"=>"Ovn",
	"le"=>"Lek"
}

def is_event_type (line)
  return $event_types[line.downcase] != nil
end

def summary_ignore (line)
  return (line.size == 10 and line =~ /[A-Z0-9][A-Z0-9][0-3][0-9][0-9][0-9XVUAN][HV][0-9][0-9][1-9]/)
end

$special_course = {} # kansli-D osv.
def is_special_course (line)
  return $special_course[line] != nil
end

event = {}
event_comments = []

cmds = []
last_cmd = []
$stdin.each_line do |line|
  if line =~ /^[A-Z]+:|;/
    cmds << last_cmd.join('')
    last_cmd = []
  end
  last_cmd << line.strip
end
cmds << last_cmd.join('')
cmds.shift # removes first nil [] thing

cmds.each do |input|
  data = input.strip().split(':',2)
  if data.size < 2
    $stderr.puts ">> warning: line without ':' character"
    next
  end

  header = data.shift
  data = data.join
#  data ~= 
  case header
  when "END" then
    if data == 'VEVENT'
      sumstr = ''
      summary = [event['name'], event['type'], event['location']]
      summary.each do |line|
        if line != nil
          sumstr += line + ' '
        end
      end
      print "SUMMARY:",sumstr.strip
      if event['code'] != nil
        print ' (', event['code'], ')'
      end

      event_comments.each do |line|
        if line.to_s != ""
          print '\n', line.to_s
        end
      end
      print "\r\n"

      if event['location'] != nil
        print 'LOCATION:', event['location'], "\r\n"
      end
      event = {}
      event_comments = []
    end
  when "SUMMARY" then
    instr = data.split /(\\n)|\\,/
    instr.each do |line|
      line.gsub! /\\n|\,|/, ''
      line.strip!
      if is_course_code line # todo: varna om det inte finns nån översättning av kurskoden samt prioritera kursinput via args
	if event['code'] == nil
	  event['code'] = line
	  event['name'] = $course_names[line]
	elsif event['code'] != line
	  if event['name'] == nil
	    event['code'] = line
	    event['name'] = $course_names[line]
          elsif event[line] != nil # warn if there are duplicate names for this course
	    $stderr.print ">> warning: event course code '", line, "' differs from previous '", event['code'], "'\n"
          end
	end
      elsif is_special_course line
        event['name'] = special_course[line]
      elsif is_event_type line
        event['type'] = $event_types[line.downcase]
      elsif summary_ignore line
      else
        event_comments << line
      end
    end
    #data = data.join
    next
  when "LOCATION" then
    event['location'] = data
    next
  end
  if header != 'SUMMARY'
    print header, ':', data,"\r\n" # same \r\n as timeedit does
  end
end

