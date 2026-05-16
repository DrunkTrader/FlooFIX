#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "storage.hpp"
#include "tokenizer.hpp"

int main(void){
    std::string raw_msg = "8=FIX.4.2|9=118|35=D|49=SENDER|56=TARGET|34=2|52=20240528-09:20:52|11=ORDERID|55=MSFT|54=1|38=1000|40=2|44=150.5|10=000|";

    // 1. Preprocess the raw message to replace '|' with SOH ('\x01')
    std::string processed_msg = fix::preprocess_delimeter(raw_msg);

    // 2. Tokenize the string into individual "tag=value" strings
    std::vector<std::string> tokens = fix::tokenize(processed_msg);

    // 3. Store the split fields into our map
    std::unordered_map<int, std::string> fixmsg_store;
    for (const auto& token : tokens) {
        fix::FixField field = fix::splitField(token);
        if (field.tag != -1) { // Only store valid fields
            fixmsg_store[field.tag] = field.value;
        }
    }

    // print the FIX message in tags and values
    for (const auto &pair : fixmsg_store)
    {
        std::cout << "Tag: " << pair.first << ", Value: " << pair.second << "\n";
    }
    return 0;
}