java -jar ../build/include/antlr-4.13.1-complete.jar -o .antlr TIP.g4 
javac -cp .:../build/include/antlr-4.13.1-complete.jar .antlr/*.java
