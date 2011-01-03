require 'net/http'
require 'cgi'

doc = ARGV[0]

$docv = []

def wikifetch(http, header, doc)
  resp = http.get("/projects/karspexet/wiki/" + doc, header)
  version = (/format=html&amp;version=(\d+)/.match(resp.body))[1]
  $docv[-1] << version
  #$stderr.puts version
  resp = http.get("/projects/karspexet/wiki/" + doc + "?format=txt", header)
  out = resp.body
  #out.sub! /h[1-4]\..*$/, '\0' + "\n\n_Genererat från revision " + version + " av \"" + doc + "\"._\n"
  out.gsub!(/\{\{include\((.*)\)\}\}/) { |s|
    $docv << [$1]
    inc = $1
    inc.gsub!(/\//, '')
    inc.gsub!(/ /, '_')
    wikifetch http, header, inc
  }
  return resp.body
end

#http://redmine.torandi.com/projects/karspexet/wiki/PPD?format=txt&version=10
Net::HTTP.start("redmine.torandi.com") { |http|
  login = http.get("/login")
  cookie = login.response['set-cookie']
  header = {'Cookie'=>cookie}

  auth_token = login.body.match(/<input[^>]name="authenticity_token"[^>]*value="([^"]*)".*\/>/m)[1]
  #req = {'username'=>'fetch', 'password'=>'woof', 'authenticity_token'=> auth_token}
  req = ['authenticity_token=' + CGI.escape(auth_token), 'username=fetch', 'password=woof' ].join '&'
  res = http.post('/login', req, header)
  cookie = res.response['set-cookie']
  header = {'Cookie'=>cookie}
  $docv << [doc]
  wikidoc = wikifetch http, header, doc
  puts 'h1. Dokumentversioner'
  puts
  puts 'Dokumentet har genererats från följande deldokument.'
  puts
  $docv.shift
  $docv.each do |elem|
    puts '*' + elem[0] + '* version: _' + elem[1] + '_.'
    puts
  end
  wikidoc.gsub!(/\r/, '')
  puts wikidoc
}
