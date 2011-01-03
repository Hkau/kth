#cd /home/axel/windows/projects/Skola/Inda/zuul/
#javac -Xlint:unchecked *.java
cd compiled
jar cvfm ../ZuulGame.jar ./manifest.txt ./org/x2d/zuul/*.class ./org/x2d/console/*.class
