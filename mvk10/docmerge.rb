require 'net/http'
require 'cgi'

doc = ARGV[0]

$docv = []

ver = open('versions.txt', 'w')

def fetchfiles(http, header, doc)
  resp = http.get("/projects/karspexet/wiki/" + doc, header)
  resp.body.gsub!(/"(\/attachments\/\d+\/(\S+))"/){ |s|
    doc = $1
    name = CGI.unescape($2)
    name.gsub!('å', 'aa')
    name.gsub!('ä', 'ae')
    name.gsub!('ö', 'oe')
    $stderr.puts doc + ': ' + name
    f = open(name, 'w')
    f << http.get(doc, header).body
    f.close()
    ''
  }
end

def wikifetch(http, header, doc)
  fetchfiles(http, header, doc)
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
  ver << "h1. Dokumentversioner\n\n"
  ver << "Dokumentet har genererats från följande deldokument.\n\n"
  $docv.shift
  $docv.sort!{ |x, y|
    x[0].casecmp(y[0])
  }
  $docv.each do |elem|
    ver << '*' + elem[0] + '* version: _' + elem[1] + '_.' + "\n\n"
  end
  wikidoc.gsub!(/\r/, '')
  puts wikidoc
}

