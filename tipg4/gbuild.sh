java -jar ../build/include/antlr-4.10.1-complete.jar -o .antlr TIP.g4 
javac -cp .:../build/include/antlr-4.10.1-complete.jar .antlr/*.java
