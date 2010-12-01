require 'csv'

csv = CSV.open('exportutf8.csv', 'r', ',')

tags = csv.shift

translation = {
  '#' => 'Kravnummer',
  'Ämne' => 'Krav',
  'Beskrivning' => 'Beskrivning',
  'Rättfärdigande' => 'Motivering',
  'Kravversion' => 'Behov',
  'Prioritet' => 'Prioritet',
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
    posts[-1][name] = val
  end
end

posts.sort! {|a,b| a['Kravnummer'].to_i <=> b['Kravnummer'].to_i }

rows = [
  'Kravnummer',
  'Krav',
  'Beskrivning',
  'Motivering',
  'Behov',
  'Prioritet',
#  'Stabilitet',
  'Källa',
  'Verifierbarhet',
]

posts.each do |post|
  rows.each do |entry|
    puts '|_.' + entry + '|' + post[entry] + '|'
  end
  puts
#  print post['Kravnummer'] + ' '
#  puts post['Krav']
end

