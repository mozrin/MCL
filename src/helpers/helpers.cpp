#include "helpers.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <iostream>
#include <stdexcept>
#include <limits>

namespace fs = std::filesystem;

static Value parseLiteralString(const std::string &s)
{
    if (s.length() >= 2 && s.front() == '"' && s.back() == '"')
    {
        std::string unquoted_s = s.substr(1, s.length() - 2);
        std::string result;
        result.reserve(unquoted_s.length());
        for (size_t i = 0; i < unquoted_s.length(); ++i)
        {
            if (unquoted_s[i] == '\\' && i + 1 < unquoted_s.length())
            {
                switch (unquoted_s[i + 1])
                {
                case '"':
                    result += '"';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                default:
                    result += unquoted_s[i + 1];
                    break;
                }
                i++;
            }
            else
            {
                result += unquoted_s[i];
            }
        }
        return result;
    }
    return s;
}

static Value parseNumericOrBooleanLiteral(const std::string &s)
{
    if (s == "true")
        return true;
    if (s == "false")
        return false;

    std::istringstream iss(s);
    long long ll_val;
    if (iss >> ll_val && iss.eof())
    {
        return ll_val;
    }

    iss.clear();
    iss.str(s);
    double d_val;
    if (iss >> d_val && iss.eof())
    {
        return d_val;
    }
    return std::monostate{};
}

static ParameterDefinition parseParameterLine(Evaluator &eval, const std::string &line)
{
    ParameterDefinition def;
    std::string param_str = line.substr(line.find('=') + 1);
    param_str = std::regex_replace(param_str, std::regex("^\\s+|\\s+$"), "");

    std::regex required_regex("<(string|integer|number|boolean)>");
    std::regex optional_regex("\\[([^\\]]+)\\]");

    std::smatch match;

    if (std::regex_match(param_str, match, required_regex))
    {
        def.kind = ParameterKind::REQUIRED;
        def.config_value_str = match[1].str();
        if (def.config_value_str == "string")
            def.expected_type = DeclaredType::STRING;
        else if (def.config_value_str == "integer")
            def.expected_type = DeclaredType::INTEGER;
        else if (def.config_value_str == "number")
            def.expected_type = DeclaredType::NUMBER;
        else if (def.config_value_str == "boolean")
            def.expected_type = DeclaredType::BOOLEAN;
        else
            throw std::runtime_error("Unknown type in required parameter: " + def.config_value_str);
    }
    else if (std::regex_match(param_str, match, optional_regex))
    {
        def.kind = ParameterKind::OPTIONAL;
        def.config_value_str = match[1].str();

        def.resolved_value = parseLiteralString(def.config_value_str);
        if (std::holds_alternative<std::string>(def.resolved_value) && std::get<std::string>(def.resolved_value) == def.config_value_str)
        {
            def.resolved_value = parseNumericOrBooleanLiteral(def.config_value_str);
        }

        if (std::holds_alternative<std::monostate>(def.resolved_value))
        {
            try
            {
                def.resolved_value = eval.getConstant(def.config_value_str);
            }
            catch (const std::runtime_error &e)
            {
                throw std::runtime_error("Helper config error: Optional parameter default '" + def.config_value_str + "' is not a valid literal or constant: " + e.what());
            }
        }
    }
    else
    {
        def.kind = ParameterKind::FIXED;
        def.config_value_str = param_str;

        def.resolved_value = parseLiteralString(def.config_value_str);
        if (std::holds_alternative<std::string>(def.resolved_value) && std::get<std::string>(def.resolved_value) == def.config_value_str)
        {
            def.resolved_value = parseNumericOrBooleanLiteral(def.config_value_str);
        }

        if (std::holds_alternative<std::monostate>(def.resolved_value))
        {
            try
            {
                def.resolved_value = eval.getConstant(def.config_value_str);
            }
            catch (const std::runtime_error &e)
            {
                throw std::runtime_error("Helper config error: Fixed parameter value '" + def.config_value_str + "' is not a valid literal or constant: " + e.what());
            }
        }
    }
    return def;
}

static HelperDefinition loadIndividualHelperConfig(Evaluator &eval, const fs::path &config_file_path)
{
    std::ifstream file(config_file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open helper config file: " + config_file_path.string());
    }

    HelperDefinition helper_def;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line.find_first_not_of(" \t\n\r") == std::string::npos)
            continue;

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos)
        {
            throw std::runtime_error("Invalid line in helper config '" + config_file_path.string() + "': " + line + ". Expected 'key=value'.");
        }

        std::string key = line.substr(0, eq_pos);
        key = std::regex_replace(key, std::regex("^\\s+|\\s+$"), "");

        if (key == "name")
        {
            helper_def.helper_name = line.substr(eq_pos + 1);
            helper_def.helper_name = std::regex_replace(helper_def.helper_name, std::regex("^\\s+|\\s+$"), "");
        }
        else if (key == "extension")
        {
            helper_def.extension_name = line.substr(eq_pos + 1);
            helper_def.extension_name = std::regex_replace(helper_def.extension_name, std::regex("^\\s+|\\s+$"), "");
        }
        else if (key.rfind("parameter_", 0) == 0)
        {
            helper_def.parameters.push_back(parseParameterLine(eval, line));
        }
        else
        {
            std::cerr << "Warning: Unknown key '" << key << "' in helper config file: " << config_file_path.string() << std::endl;
        }
    }

    if (helper_def.helper_name.empty())
    {
        throw std::runtime_error("Helper config '" + config_file_path.string() + "' is missing 'name' field.");
    }
    if (helper_def.extension_name.empty())
    {
        throw std::runtime_error("Helper config '" + config_file_path.string() + "' is missing 'extension' field.");
    }

    return helper_def;
}

static std::vector<HelperDefinition> loadHelperDefinitions(Evaluator &eval, const std::string &system_helpers_dir)
{
    std::vector<HelperDefinition> definitions;
    fs::path helpers_path = system_helpers_dir;

    if (!fs::exists(helpers_path) || !fs::is_directory(helpers_path))
    {
        throw std::runtime_error("System helpers directory not found: " + helpers_path.string());
    }

    for (const auto &entry : fs::directory_iterator(helpers_path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".config")
        {
            HelperDefinition def = loadIndividualHelperConfig(eval, entry.path());
            definitions.push_back(def);
        }
    }
    return definitions;
}

void registerAllHelpers(Evaluator &eval)
{
    std::vector<HelperDefinition> helper_defs = loadHelperDefinitions(eval, "src/helpers/system");

    for (const auto &h_def : helper_defs)
    {
        eval.registerNativeFunction(h_def.helper_name,
                                    [&eval, h_def](const std::vector<Value> &user_args) -> Value
                                    {
                                        std::vector<Value> extension_args;
                                        size_t user_arg_idx = 0;
                                        size_t required_count = 0;
                                        size_t optional_count = 0;

                                        for (const auto &param_def : h_def.parameters)
                                        {
                                            if (param_def.kind == ParameterKind::REQUIRED)
                                            {
                                                required_count++;
                                            }
                                            else if (param_def.kind == ParameterKind::OPTIONAL)
                                            {
                                                optional_count++;
                                            }
                                        }

                                        if (user_args.size() < required_count || user_args.size() > (required_count + optional_count))
                                        {
                                            std::stringstream ss;
                                            ss << "Helper '" << h_def.helper_name << "' expects between " << required_count << " and " << (required_count + optional_count) << " arguments, but received " << user_args.size() << ".";
                                            throw std::runtime_error(ss.str());
                                        }

                                        for (const auto &param_def : h_def.parameters)
                                        {
                                            if (param_def.kind == ParameterKind::REQUIRED)
                                            {
                                                if (user_arg_idx >= user_args.size())
                                                {
                                                    throw std::runtime_error("Internal error: Missing required argument for helper '" + h_def.helper_name + "'.");
                                                }
                                                if (param_def.expected_type == DeclaredType::STRING)
                                                {
                                                    extension_args.push_back(get_string_arg(user_args, user_arg_idx, h_def.helper_name, "user_arg_" + std::to_string(user_arg_idx + 1)));
                                                }
                                                else if (param_def.expected_type == DeclaredType::INTEGER)
                                                {
                                                    extension_args.push_back(get_integer_arg(user_args, user_arg_idx, h_def.helper_name, "user_arg_" + std::to_string(user_arg_idx + 1)));
                                                }
                                                else if (param_def.expected_type == DeclaredType::NUMBER)
                                                {
                                                    extension_args.push_back(get_number_arg(user_args, user_arg_idx, h_def.helper_name, "user_arg_" + std::to_string(user_arg_idx + 1)));
                                                }
                                                else if (param_def.expected_type == DeclaredType::BOOLEAN)
                                                {
                                                    if (!std::holds_alternative<bool>(user_args[user_arg_idx]))
                                                    {
                                                        throw std::runtime_error("Helper '" + h_def.helper_name + "': Argument " + std::to_string(user_arg_idx + 1) + " must be a boolean.");
                                                    }
                                                    extension_args.push_back(user_args[user_arg_idx]);
                                                }
                                                user_arg_idx++;
                                            }
                                            else if (param_def.kind == ParameterKind::OPTIONAL)
                                            {
                                                if (user_arg_idx < user_args.size())
                                                {
                                                    extension_args.push_back(user_args[user_arg_idx]);
                                                    user_arg_idx++;
                                                }
                                                else
                                                {
                                                    extension_args.push_back(param_def.resolved_value);
                                                }
                                            }
                                            else
                                            {
                                                extension_args.push_back(param_def.resolved_value);
                                            }
                                        }

                                        return eval.callNativeFunctionByName(h_def.extension_name, extension_args);
                                    });
    }
}