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
    val.gsub! /(\s|\.)+$/, '' # remove trailing periods
    if name != 'Krav' and name != '#'
      val.gsub! /$/, '.' # one last period.
    end
    posts[-1][name] = val
  end
end

postcat = {}

posts.each do |post|
  if postcat[post['Kategori']] == nil
    postcat[post['Kategori']] = []
  end
  postcat[post['Kategori']] << post
end

postcat.each do |cat, posts|
  posts.sort! {|a,b|
    a['Krav'] <=> b['Krav']
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

tags.each do |cat|
  posts = postcat[cat]
  puts 'h3. ' + cat
  puts
  posts.each do |post|
    puts '|_.' + 'Krav' + '|' + post['Krav'].strip + ' (_#' + post['#'] + '_)|'
    rows.each do |entry|
      puts '|_.' + entry + '|' + post[entry] + '|'
    end
    puts
  end
end

