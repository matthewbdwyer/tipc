# TIP grammar

Once you have built the tipc project the antlr4 library will be installed in:
`../build/include/antlr-4.10.1-complete.jar`

To build and run it using ANTLR4 execute:
 1. java -jar ../build/include/antlr-4.10.1-complete.jar  TIP.g4 
 1. javac -cp .:../build/include/antlr-4.10.1-complete.jar *.java
 1. java -cp .:../build/include/antlr-4.10.1-complete.jar org.antlr.v4.gui.TestRig TIP program < <a TIP program>

This directory includes two scripts `gbuild.sh` which runs the first two steps and `grun.sh` which runs the second.
