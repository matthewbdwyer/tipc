add_library(utilsLib STATIC 
            PrettyPrinter.h
            PrettyPrinter.cpp
            SampleVisitor.h
            SampleVisitor.cpp)
target_link_libraries(utilsLib tipLib)
