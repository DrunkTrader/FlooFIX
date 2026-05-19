# FlooFIX

This is a Fix Parser

## Build and Run

Use a C++17 compiler. On Windows with MinGW:

```powershell
g++ -std=c++17 -Iinc main.cpp src\parser.cpp src\storage.cpp src\tokenizer.cpp src\validator.cpp -o FlooFIX.exe
```

Run:

```powershell
./FlooFIX.exe
```

The sample message is defined in `main.cpp` as `raw_msg`. Validation output is printed after the parsed tags.

## Done

1. Define a `FixField` structure with `int tag` and `std::string value` to represent a single FIX field.

2. Define a `FixMessage` container using `std::map<int, std::string>` (or `std::unordered_map` if you do not need ordering) to store parsed fields.

3. Create a `Tokenizer` module that splits the raw message using the SOH delimiter (`\x01`).

4. Add a preprocessing step to replace `|` with `\x01` so you can test easily with readable input.
5. Implement a `splitField()` function that divides each token into `tag` and `value` using `=`.

6. Convert tag strings to integers using `std::stoi`, and handle invalid conversions safely.

7. Insert parsed fields into the `FixMessage` map; decide how to handle duplicate tags (overwrite or reject).

8. Create a `Parser` class with a `parse(std::string raw)` method that orchestrates tokenization and field extraction.

9. Implement header validation to check mandatory tags like `8, 9, 35, 49, 56, 34, 52`.

10. Implement body validation using message-type rules based on tag `35` (e.g., `D`, `A`, `8`).

11. Write a function to compute checksum (sum of bytes % 256) and validate it against tag `10`.

## To-Do's

12. Implement body length validation by verifying tag `9` matches the actual message body length.

13. Create a `ValidationResult` or error-reporting mechanism to return precise failure reasons.

14. Add a formatter utility to print parsed messages in readable or JSON-like form for debugging.

15. Write unit tests with valid and malformed FIX messages, then refactor the parser into clean modules (`Tokenizer`, `Parser`, `Validator`) so it can later evolve into a FIX engine.
