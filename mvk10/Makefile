all:
	echo 'make urd', 'make srd' or 'make add'


urd:
	mkdir -p files
	ruby1.8 fetchissues.rb > export.csv
	iconv -f iso8859-1 export.csv > exportutf8.csv
	ruby1.8 issueformat.rb < exportutf8.csv > issues.txt
	ruby1.8 docmerge.rb --doc=User_Requirements_Document > urd.txt
	ruby1.8 wikitex.rb $(pre) < urd.txt > urd.tex
	pdflatex urd.tex
	pdflatex urd.tex
	rm export.csv
	rm exportutf8.csv
	rm issues.txt
	rm urd.aux
	rm urd.log
	rm urd.toc
	rm versions.txt
	rm -r files/
add:
	mkdir -p files
	ruby1.8 docmerge.rb --doc=Architectural_Design_Document > add.txt
	ruby1.8 wikitex.rb $(pre) < add.txt > add.tex
	pdflatex add.tex
	pdflatex add.tex
	rm add.aux
	rm add.log
	rm add.toc
	rm versions.txt
	rm -r files/

srd:
	mkdir -p files
	ruby1.8 docmerge.rb --doc=Software_Requirements_Document > srd.txt
	ruby1.8 wikitex.rb $(pre) < srd.txt > srd.tex
	pdflatex srd.tex
	pdflatex srd.tex
	rm srd.aux
	rm srd.log
	rm srd.toc
	rm versions.txt
	rm -r files/

pre_urd:
	./build_pre User_Requirements_Document urd

pre_srd:
	./build_pre Software_Requirements_Document srd

pre_add:
	./build_pre Architectural_Design_Document add

pre: pre_urd pre_srd pre_add
