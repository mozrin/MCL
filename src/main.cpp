#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <exception>
#include <vector>
#include <filesystem>
#include <regex>

#include "core/lexer.h"
#include "core/parser.h"
#include "core/ast.h"
#include "core/token.h"
#include "core/evaluator.h"
#include "core/constants.h"
#include "core/debugger.h"
#include "extensions/extensions.h"

namespace fs = std::filesystem;

int process_single_file(const std::string &filename)
{
    debug_print_message("Processing file: '" + filename + "'...");

    std::string source_code;
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        source_code = buffer.str();
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open file '" << filename << "'. Skipping.\n";
        return 1;
    }

    Lexer lexer(source_code);
    Parser parser(lexer);

    debug_print_message("Parsing file: '" + filename + "'...");

    std::unique_ptr<ProgramNode> ast;
    try
    {
        ast = parser.parseProgram();
        debug_print_message("Parsing finished for '" + filename + "'.");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Parser/Lexer Error in '" << filename << "': " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred during parsing '" << filename << "': " << e.what() << std::endl;
        return 1;
    }

    debug_print_ast_header("Abstract Syntax Tree (AST) for " + filename);
    debug_print_ast_node(ast.get());
    debug_print_ast_footer();

    debug_print_message("Starting interpretation for '" + filename + "'...");
    Evaluator evaluator;
    registerAllExtensions(evaluator);

    evaluator.registerConstant("MCL_UPPERCASE_EVERYTHING", (long long)MCL_UPPERCASE_EVERYTHING);
    evaluator.registerConstant("MCL_UPPERCASE_TITLE", (long long)MCL_UPPERCASE_TITLE);
    evaluator.registerConstant("MCL_UPPERCASE_FIRST", (long long)MCL_UPPERCASE_FIRST);
    evaluator.registerConstant("MCL_UPPERCASE_ALTERNATING", (long long)MCL_UPPERCASE_ALTERNATING);
    evaluator.registerConstant("MCL_UPPERCASE_TOGGLE", (long long)MCL_UPPERCASE_TOGGLE);

    try
    {
        evaluator.interpret(std::move(ast));
        debug_print_message("Interpretation finished successfully for '" + filename + "'.");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Runtime Error in '" << filename << "': " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred during interpretation of '" << filename << "': " << e.what() << std::endl;
        return 1;
    }

    debug_print_message("Finished processing '" + filename + "'.");
    return 0;
}

std::vector<std::string> get_files_from_pattern(const std::string &pattern_arg)
{
    std::vector<std::string> files;
    fs::path p(pattern_arg);

    bool contains_wildcard = pattern_arg.find('*') != std::string::npos || pattern_arg.find('?') != std::string::npos;

    if (contains_wildcard)
    {
        fs::path directory = p.has_parent_path() ? p.parent_path() : fs::current_path();
        std::string filename_pattern_str = p.filename().string();

        std::string regex_pattern_str = filename_pattern_str;
        regex_pattern_str = std::regex_replace(regex_pattern_str, std::regex("[\\.^$|()\\[\\]{}+?]"), "\\$&");
        regex_pattern_str = std::regex_replace(regex_pattern_str, std::regex("\\*"), ".*");
        regex_pattern_str = std::regex_replace(regex_pattern_str, std::regex("\\?"), ".");
        std::regex file_regex(regex_pattern_str);

        if (!fs::exists(directory))
        {
            std::cerr << "Warning: Directory '" << directory.string() << "' specified in pattern '" << pattern_arg << "' does not exist. Skipping.\n";
            return files;
        }
        if (!fs::is_directory(directory))
        {
            std::cerr << "Warning: Path '" << directory.string() << "' specified in pattern '" << pattern_arg << "' is not a directory. Skipping.\n";
            return files;
        }

        try
        {
            for (const auto &entry : fs::directory_iterator(directory))
            {
                if (entry.is_regular_file())
                {
                    if (std::regex_match(entry.path().filename().string(), file_regex))
                    {
                        files.push_back(entry.path().string());
                    }
                }
            }
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "Filesystem error while iterating directory '" << directory.string() << "': " << e.what() << std::endl;
        }
    }
    else
    {
        if (fs::exists(p) && fs::is_regular_file(p))
        {
            files.push_back(pattern_arg);
        }
        else
        {
            std::cerr << "Error: File '" << pattern_arg << "' not found or is not a regular file. Skipping.\n";
        }
    }
    return files;
}

int main(int argc, char *argv[])
{
    debug_print_message("Nirvana MVI starting (Lexer + Parser + Evaluator)...");

    std::vector<std::string> files_to_run;

    if (argc == 1)
    {
        files_to_run = get_files_from_pattern("bin/*.nv");
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            std::vector<std::string> matched_files = get_files_from_pattern(argv[i]);
            files_to_run.insert(files_to_run.end(), matched_files.begin(), matched_files.end());
        }
    }

    if (files_to_run.empty())
    {
        std::cerr << "No Nirvana (.nv) files found to process based on the provided arguments.\n";
        if (argc == 1)
        {
            std::cerr << "Attempted to find files in 'bin/*.nv'. Ensure files exist in the 'bin/' folder relative to your current directory.\n";
        }
        else
        {
            std::cerr << "Please check your file paths and patterns. No matching files were found.\n";
        }
        return 1;
    }

    int overall_status = 0;
    for (const std::string &filename : files_to_run)
    {
        int file_status = process_single_file(filename);
        if (file_status != 0)
        {
            overall_status = file_status;
            break;
        }
    }

    return overall_status;
}