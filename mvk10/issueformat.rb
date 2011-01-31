require 'csv'

csv = CSV.open('exportutf8.csv', 'r', ',')

tags = csv.shift

#,Status,Project,Tracker,Priority,Subject,Assigned to,Category,Target version,Author,Start,Due date,% Done,Estimated time,Parent task,Created,Updated,Kravversion,Kravkälla,Verifiering av kravet,Stabilitet,Rättfärdigande,Description
translation = {
  '#' => '#',
  'Subject' => 'Krav',
  'Description' => 'Beskrivning',
  'Rättfärdigande' => 'Motivering',
  'Kravversion' => 'Behov',
  'Priority' => 'Prioritet',
#  'Kravet kan komma att ändras' => 'Stabilitet',
  'Kravkälla' => 'Källa',
  'Verifiering av kravet' => 'Verifierbarhet',
}

posts = []
csv.each do |entry|
  posts << {}
  entry.each_with_index do |val, i|
    name = translation[tags[i]] == nil ? tags[i] : translation[tags[i]]
    val.gsub! /\s+/, ' '
    val.gsub! /(\s|\.)+$/, '' # remove trailing periods
    if name != 'Krav' and name != '#'
      val.gsub! /$/, '.' # one last period.
    end
    posts[-1][name] = val
  end
end

postcat = {}

posts.each do |post|
  if postcat[post['Category']] == nil
    postcat[post['Category']] = []
  end
  postcat[post['Category']] << post
end

postcat.each do |cat, posts|
  posts.sort! {|a,b|
    a['#'] <=> b['#']
  }
end

tags = []
postcat.each do |cat, posts|
  tags << cat
end

tags.sort!

rows = [
#  '#',
#  'Krav',
  'Beskrivning',
  'Motivering',
  'Behov',
  'Prioritet',
  'Stabilitet',
  'Källa',
  'Verifierbarhet',
]

i = 1
tags.each do |cat|
  posts = postcat[cat]
  puts 'h3. ' + cat
  puts
  j = 1
  posts.each do |post|
    puts '|_.' + 'Krav' + '|_.UR' +i.to_s + '.' + j.to_s + ' ' + post['Krav'].strip + ' |'
    rows.each do |entry|
      puts '|_.' + entry + '|' + post[entry] + '|'
    end
    puts
    j += 1
  end
  i += 1
end

