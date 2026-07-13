#include "test_utils.h"
#include "../src/utils.h"
#include <iostream>
#include <vector>

bool test_role_before_content() {
    std::vector<std::string> images;
    auto messages = cactus::ffi::parse_messages_json(
        R"([{"role":"user","content":"hello"}])", images);

    bool passed = messages.size() == 1 &&
                  messages[0].role == "user" &&
                  messages[0].content == "hello";
    if (!passed) {
        std::cerr << "[✗] role-before-content: role=[" << (messages.empty() ? "" : messages[0].role)
                   << "] content=[" << (messages.empty() ? "" : messages[0].content) << "]\n";
    }
    return passed;
}

bool test_content_before_role() {
    std::vector<std::string> images;
    auto messages = cactus::ffi::parse_messages_json(
        R"([{"content":"hello","role":"user"}])", images);

    bool passed = messages.size() == 1 &&
                  messages[0].role == "user" &&
                  messages[0].content == "hello";
    if (!passed) {
        std::cerr << "[✗] content-before-role: role=[" << (messages.empty() ? "" : messages[0].role)
                   << "] content=[" << (messages.empty() ? "" : messages[0].content) << "]\n";
    }
    return passed;
}

bool test_multi_message_mixed_order() {
    std::vector<std::string> images;
    auto messages = cactus::ffi::parse_messages_json(
        R"([{"role":"user","content":"first"},{"content":"second","role":"assistant"}])", images);

    bool passed = messages.size() == 2 &&
                  messages[0].role == "user" && messages[0].content == "first" &&
                  messages[1].role == "assistant" && messages[1].content == "second";
    if (!passed) {
        std::cerr << "[✗] multi-message-mixed-order: got " << messages.size() << " messages\n";
        for (const auto& m : messages) {
            std::cerr << "    role=[" << m.role << "] content=[" << m.content << "]\n";
        }
    }
    return passed;
}

int main() {
    TestUtils::TestRunner runner("JSON Message Parsing Tests");
    runner.run_test("role before content", test_role_before_content());
    runner.run_test("content before role", test_content_before_role());
    runner.run_test("multi-message mixed order", test_multi_message_mixed_order());
    runner.print_summary();
    return runner.all_passed() ? 0 : 1;
}
