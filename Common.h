#pragma once

#include <string>

const std::string message_delimiter = "\n";
const std::string data_delimiter = ":";

const std::string check_session_message = "is session canceled";
const std::string session_canceled_message = "cancel_true";
const std::string session_alive_message = "cancel_false";

enum class task_category : uint32_t
{
    slow,
    fast
};

inline std::string to_str(task_category e)
{
    switch (e)
    {
        case task_category::slow: return "slow";
        case task_category::fast: return "fast";
        default: return "unknown";
    }
}