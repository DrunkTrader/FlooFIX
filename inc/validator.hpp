#pragma once

#include <string>
#include <vector>
#include "storage.hpp"

namespace fix {
    struct ValidationResult {
        bool ok = true;
        std::vector<std::string> errors;
    };

    class Validator {
    public:
        ValidationResult validate(const std::string &raw_msg, const FixMessage &message) const;

    private:
        void validate_header(const FixMessage &message, ValidationResult &result) const;
        void validate_body(const FixMessage &message, ValidationResult &result) const;
        void validate_checksum(const std::string &raw_msg, const FixMessage &message, ValidationResult &result) const;
        int compute_checksum(const std::string &processed_msg) const;
    };
}
