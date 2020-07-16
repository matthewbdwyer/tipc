#include "catch.hpp"
#include "ASTHelper.h"
#include "AppearingFieldsVisitor.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

TEST_CASE("AppearingFieldsVisitor: Test all fields are found", "[AppearingFieldsVisitor]") {
    std::stringstream stream;
    stream << R"(
      main(){
        var rec1, rec2;
        rec1 = {a:1, b:2, c:3};
        rec2 = {d:4, e:5, f:6};
        return 0;
      }
    )";

    auto ast = ASTHelper::build_ast(stream);
    AppearingFieldsVisitor appearingFieldsVisitor;
    ast->accept(&appearingFieldsVisitor);

    auto temp = appearingFieldsVisitor.getFields();
    std::vector<std::string> actualFields(temp.begin(), temp.end());
    std::vector<std::string> expectedFields { "a", "b", "c", "d", "e", "f"};
    for(int i = 0; i < actualFields.size(); i++) {
        REQUIRE(expectedFields.at(i) == actualFields.at(i));
    }
}

