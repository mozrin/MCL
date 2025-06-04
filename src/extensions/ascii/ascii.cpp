#include "ascii.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>

Value mcl_ascii(const std::vector<Value> &args)
{
if (args.size() != 1)
{
throw std::runtime_error("Function 'ascii' expects exactly 1 argument: ascii(string $character).");
}

std::string input_string;
if (std::holds_alternative<std::string>(args[0]))
{
input_string = std::get<std::string>(args[0]);
}
else
{
std::stringstream ss;
ss << "Function 'ascii': Argument 1 ($character) must be a string, but got value of type ";
switch (valueTypeToDeclaredType(args[0]))
{
case DeclaredType::INTEGER:
ss << "integer";
break;
case DeclaredType::NUMBER:
ss << "number";
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

if (input_string.empty())
{
throw std::runtime_error("Function 'ascii': Argument 1 ($character) must not be an empty string.");
}

return static_cast<long long>(static_cast<unsigned char>(input_string[0]));
}

void register_ascii_extension(Evaluator &eval)
{
eval.registerNativeFunction("ascii", mcl_ascii);
}