require 'net/http'
require 'cgi'

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
  print http.get('/projects/karspexet/issues.csv', header).body
}

