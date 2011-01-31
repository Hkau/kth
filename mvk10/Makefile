urd:
	ruby1.8 fetchissues.rb > export.csv
	iconv -f iso8859-1 export.csv > exportutf8.csv
	ruby1.8 issueformat.rb < exportutf8.csv > issues.txt
	ruby1.8 docmerge.rb User_Requirements_Document > urd.txt
	ruby1.8 wikitex.rb < urd.txt > urd.tex
	pdflatex urd.tex
	pdflatex urd.tex
	rm export.csv
	rm exportutf8.csv
	rm issues.txt
	rm urd.aux
	rm urd.log
	rm urd.toc
	rm versions.txt

srd:
	ruby1.8 docmerge.rb Software_Requirements_Document > srd.txt
	ruby1.8 wikitex.rb < srd.txt > srd.tex
	pdflatex srd.tex
	pdflatex srd.tex
	rm srd.aux
	rm srd.log
	rm srd.toc
	rm versions.txt

