require 'net/http'
require 'cgi'

doc=nil
$pre=nil
ARGV.each do |arg|
	if arg =~/--doc=(.+)/ or arg =~/-d(.+)/
		doc=$1
	end

	if arg =~/--pre=(.+)/ or arg =~/-p(.+)/
		$pre=$1
	end
end

if doc.nil?
	$stderr.write "Missing --doc, crashing, crashing!\n"
	exit
end

$docv = []


def fetchfiles(http, header, doc)
  resp = http.get("/projects/karspexet/wiki/" + doc, header)
  resp.body.gsub!(/"(\/attachments\/\d+\/(\S+))"/){ |s|
    doc = $1
    name = CGI.unescape($2)
    name.gsub!('å', 'aa')
    name.gsub!('ä', 'ae')
    name.gsub!('ö', 'oe')
    $stderr.puts doc + ': files/' + name
    f = open('files/' + name, 'w')
    f << http.get(doc, header).body
    f.close()
    ''
  }
end

def wikifetch(http, header, doc)
	if $pre.nil?
		fetchfiles(http, header, doc)
	end
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


	if $pre.nil?
		ver = open('versions.txt', 'w')
		ver << "h1. Dokumentversioner\n\n"
		ver << "Dokumentet har genererats från följande deldokument.\n\n"
		$docv.shift
		$docv.each do |elem|
			ver << '*' + elem[0] + '* version: _' + elem[1] + '_.' + "\n\n"
		end
		ver.close

		wikidoc.gsub!(/\r/, '')
		puts wikidoc
	else
		#Just check versions and exit with correct exit status
		cur_ver=open("#{$pre}_versions.txt").read.split("\n")
		$docv.shift
		if cur_ver.size==$docv.size
			if cur_ver.zip($docv).all? { |m| m[0]=="#{m[1][0]}:#{m[1][1]}" } 
				exit 0
			end
		end
		#Doesn't match, update #{$pre}_versions.txt
		ver=open("#{$pre}_versions.txt",'w')
		$docv.each do |elem| 
			ver<<"#{elem[0]}:#{elem[1]}\n"
		end
		exit 1
	end
}

