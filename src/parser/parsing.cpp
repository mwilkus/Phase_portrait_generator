#include "parsing.h"
#include <cmath>
#include <mutex>

void parse_input(std::string* input_x, std::string* input_y) {
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x", equations::x);
    symbol_table.add_variable("y", equations::y);
    symbol_table.add_constants();
    symbol_table.add_constant("e", std::exp(1.0));
    exprtk::expression<double> expression_x;
    expression_x.register_symbol_table(symbol_table);
    exprtk::expression<double> expression_y;
    expression_y.register_symbol_table(symbol_table);
    exprtk::parser<double> parser;
    equations::valid = (parser.compile(*input_x, expression_x)&&
    parser.compile(*input_y, expression_y));
    equations::x_expr = expression_x;
    equations::y_expr = expression_y;
}
