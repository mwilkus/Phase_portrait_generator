#include "parsing.h"

void parse_input(std::string* input_x, std::string* input_y) {
    exprtk::symbol_table<float> symbol_table;
    symbol_table.add_variable("x", equations::x);
    symbol_table.add_variable("y", equations::y);
    exprtk::expression<float> expression_x;
    expression_x.register_symbol_table(symbol_table);
    exprtk::expression<float> expression_y;
    expression_y.register_symbol_table(symbol_table);
    exprtk::parser<float> parser;
    equations::valid = (parser.compile(*input_x, expression_x)&&
    parser.compile(*input_y, expression_y));
    equations::x_expr = expression_x;
    equations::y_expr = expression_y;
}
