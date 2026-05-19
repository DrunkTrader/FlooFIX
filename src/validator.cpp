#include "validator.hpp"
#include "tokenizer.hpp"
#include <unordered_map>
#include <sstream>

namespace fix {
    ValidationResult Validator::validate(const std::string &raw_msg, const FixMessage &message) const {
        ValidationResult result;
        validate_header(message, result);
        validate_body(message, result);
        validate_checksum(raw_msg, message, result);
        result.ok = result.errors.empty();
        return result;
    }

    void Validator::validate_header(const FixMessage &message, ValidationResult &result) const {
        const int required_tags[] = {8, 9, 35, 49, 56, 34, 52};
        for (const int tag : required_tags) {
            if (message.find(tag) == message.end()) {
                result.errors.push_back("Missing required header tag: " + std::to_string(tag));
            }
        }
    }

    void Validator::validate_body(const FixMessage &message, ValidationResult &result) const {
        auto msg_type_it = message.find(35);
        if (msg_type_it == message.end()) {
            result.errors.push_back("Missing MsgType (35) for body validation");
            return;
        }

        const std::string &msg_type = msg_type_it->second;
            static const std::unordered_map<std::string, std::vector<int>> required_by_type = {
            {"D", {11, 55, 54, 38, 40}},
            {"A", {98, 108}},
            {"8", {37, 17, 39, 150, 55, 54, 38}}
        };

        auto rule_it = required_by_type.find(msg_type);
        if (rule_it == required_by_type.end()) {
            result.errors.push_back("No body validation rules for MsgType: " + msg_type);
            return;
        }

        for (const int tag : rule_it->second) {
            if (message.find(tag) == message.end()) {
                result.errors.push_back("Missing required body tag for MsgType " + msg_type + ": " + std::to_string(tag));
            }
        }
    }

    void Validator::validate_checksum(const std::string &raw_msg, const FixMessage &message, ValidationResult &result) const {
        auto checksum_it = message.find(10);
        if (checksum_it == message.end()) {
            result.errors.push_back("Missing checksum tag (10)");
            return;
        }

        std::string processed = preprocess_delimeter(raw_msg);
        int expected_checksum = compute_checksum(processed);
        if (expected_checksum < 0) {
            result.errors.push_back("Could not compute checksum (missing tag 10 in raw message)");
            return;
        }

        int provided_checksum = -1;
        try {
            provided_checksum = std::stoi(checksum_it->second);
        } catch (const std::exception &) {
            result.errors.push_back("Invalid checksum value in tag 10");
            return;
        }

        if (expected_checksum != provided_checksum) {
            std::ostringstream oss;
            oss << "Checksum mismatch: expected " << expected_checksum << ", got " << provided_checksum;
            result.errors.push_back(oss.str());
        }
    }

    int Validator::compute_checksum(const std::string &processed_msg) const {
        const std::string checksum_tag = "10=";
        const std::size_t tag_pos = processed_msg.find(checksum_tag);
        if (tag_pos == std::string::npos) {
            return -1;
        }

        int sum = 0;
        for (std::size_t i = 0; i < tag_pos; ++i) {
            sum += static_cast<unsigned char>(processed_msg[i]);
        }
        return sum % 256;
    }
}
