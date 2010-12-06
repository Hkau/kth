require 'csv'

csv = CSV.open('exportutf8.csv', 'r', ',')

tags = csv.shift

translation = {
  '#' => '#',
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

posts.sort! {|a,b| a['#'].to_i <=> b['#'].to_i }

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

posts.each do |post|
  puts '|_.' + 'Krav' + '|' + post['Krav'] + ' (_#' + post['#'] + '_)|'
  rows.each do |entry|
    puts '|_.' + entry + '|' + post[entry] + '|'
  end
  puts
end

