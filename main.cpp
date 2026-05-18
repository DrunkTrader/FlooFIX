#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "inc/parser.hpp"
#include "inc/validator.hpp"

int main(void){
    std::string raw_msg = "8=FIX.4.2|9=118|35=D|49=Dhruv|56=TARGET|34=2|52=20240528-09:20:52|11=ORDERID|55=MSFT|54=1|38=1000|40=2|44=150.5|10=000|";

    fix::Parser parser;
    auto fixmsg_store = parser.parse(raw_msg);
    fix::Validator validator;
    auto validation = validator.validate(raw_msg, fixmsg_store);

    // print the FIX message in tags and values
    for (const auto &pair : fixmsg_store)
    {
        std::cout << "Tag: " << pair.first << ", Value: " << pair.second << "\n";
    }

    if (validation.ok) {
        std::cout << "Validation: OK\n";
    } else {
        std::cout << "Validation errors:\n";
        for (const auto &error : validation.errors) {
            std::cout << " - " << error << "\n";
        }
    }
    return 0;
}