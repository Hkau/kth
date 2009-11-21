function x = tridia(d,p,q,b)    
% Ber�knar l�sningsvektorn x till tridiagonalt system
% med diagonal d, superdiagonal p, subdiagonal q och h�gerled b
    n=length(b);  r=d; g=b; x=b;      
    for i=2:n
      i1=i-1;  mult=q(i1)/r(i1);          % Gausseliminera 
      r(i)=d(i)-mult*p(i1); g(i)=b(i)-mult*g(i1);
    end
    x(n)=g(n)/r(n);                       % Bak�tsubstituera
    for i=n-1:-1:1, x(i)=(g(i)-p(i)*x(i+1))/r(i); end


