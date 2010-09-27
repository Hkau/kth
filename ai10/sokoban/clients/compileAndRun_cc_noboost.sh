# requires boost system library and includes for array and asio
code_noboost='client_noboost.cc'
if [ ! -f $code_noboost ]
then
   ln -s client.c $code_noboost
fi
g++ $code_noboost -o client_noboost_cc
./client_noboost_cc 1
