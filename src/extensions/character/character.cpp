#include "character.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <cmath>

Value mcl_character(const std::vector<Value> &args)
{
if (args.size() != 1)
{
throw std::runtime_error("Function 'character' expects exactly 1 argument: character(integer $ascii).");
}

long long ascii_val;
if (std::holds_alternative<long long>(args[0]))
{
ascii_val = std::get<long long>(args[0]);
}
else if (std::holds_alternative<double>(args[0]))
{
double d_val = std::get<double>(args[0]);
if (d_val != std::floor(d_val))
{
std::stringstream ss;
ss << "Function 'character': Argument 1 ($ascii) must be an integer, but got " << d_val << ".";
throw std::runtime_error(ss.str());
}
ascii_val = static_cast<long long>(d_val);
}
else
{
std::stringstream ss;
ss << "Function 'character': Argument 1 ($ascii) must be an integer (integer or number), but got value of type ";
switch (valueTypeToDeclaredType(args[0]))
{
case DeclaredType::STRING:
ss << "string";
break;
case DeclaredType::BOOLEAN:
ss << "boolean";
break;
default:
ss << "unknown";
break;
}
ss << ".";
throw std::runtime_error(ss.str());
}

if (ascii_val < 0 || ascii_val > 255)
{
std::stringstream ss;
ss << "Function 'character': ASCII value " << ascii_val << " is out of valid character range (0-255).";
throw std::runtime_error(ss.str());
}

return std::string(1, static_cast<char>(ascii_val));
}

void register_character_extension(Evaluator &eval)
{
eval.registerNativeFunction("character", mcl_character);
}