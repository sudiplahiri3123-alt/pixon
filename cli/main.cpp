#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "../core/include/pixon_c_api.h"


struct Operation {
    std::string name;
    std::vector<double> params; // param1, param2, param3...
    std::string str_param;      // e.g., path for composite
    double opacity = 1.0;
    int width = 0;
    int height = 0;
    int x = 0; // for crop/extend
    int y = 0; // for crop/extend
};


OperationC to_c_operation(const Operation& op) {
    OperationC c_op = {};
    c_op.name   = op.name.c_str();
    c_op.width  = op.width;
    c_op.height = op.height;
    c_op.x      = op.x;
    c_op.y      = op.y;
    c_op.path   = op.str_param.empty() ? nullptr : op.str_param.c_str();

    if (op.name == "resize") {
        c_op.param1 = static_cast<double>(op.width);
        c_op.param2 = static_cast<double>(op.height);
        c_op.param3 = op.params.empty() ? 0.0 : op.params[0]; // crop flag
    } 
    else if (op.name == "crop" || op.name == "extend") {
        if (op.params.size() >= 4) {
            c_op.x      = static_cast<int>(op.params[0]);
            c_op.y      = static_cast<int>(op.params[1]);
            c_op.width  = static_cast<int>(op.params[2]);
            c_op.height = static_cast<int>(op.params[3]);
        }
    } 
    else if (op.name == "composite") {
        if (op.params.size() >= 2) {
            c_op.x      = static_cast<int>(op.params[0]);
            c_op.y      = static_cast<int>(op.params[1]);
            c_op.param1 = op.opacity;
        }
    } 
    else {
        c_op.param1 = op.params.size() > 0 ? op.params[0] : 0.0;
        c_op.param2 = op.params.size() > 1 ? op.params[1] : 0.0;
        c_op.param3 = op.params.size() > 2 ? op.params[2] : 0.0;
    }

    return c_op;
}


bool is_operation(const std::string& token) {
    static const std::vector<std::string> ops = {
        "resize","rotate","crop","blur","sharpen","flip","flop","coloradjust",
        "composite","addalpha","removealpha","extend","trim"
    };
    return std::find(ops.begin(), ops.end(), token) != ops.end();
}

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(),
        [](char c){ return std::isdigit(c) || c == '.' || c == '-'; });
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: pixon <input> [operations...] <output>\n";
        return 1;
    }

    std::string input_path  = argv[1];
    std::string output_path = argv[argc - 1];
    std::vector<Operation> operations;

  
    for (int i = 2; i < argc - 1;) {
        std::string op_name = argv[i++];
        Operation op;
        op.name = op_name;

        if (op_name == "resize") {
            if (i + 1 >= argc - 1) { std::cerr << "Error: resize requires width and height\n"; return 1; }
            op.width  = std::stoi(argv[i++]);
            op.height = std::stoi(argv[i++]);
            if (i < argc - 1 && is_number(argv[i])) op.params.push_back(std::stod(argv[i++])); // optional crop flag
        } 
        else if (op_name == "rotate" || op_name == "blur" || op_name == "sharpen") {
            op.params.push_back(i < argc - 1 && is_number(argv[i]) ? std::stod(argv[i++]) : 1.0);
        } 
        else if (op_name == "crop" || op_name == "extend") {
            if (i + 3 >= argc - 1) { std::cerr << op_name << " requires 4 parameters\n"; return 1; }
            for (int j = 0; j < 4; ++j) op.params.push_back(std::stod(argv[i++]));
        } 
        else if (op_name == "coloradjust") {
            if (i + 2 >= argc - 1) { std::cerr << "coloradjust requires 3 parameters\n"; return 1; }
            for (int j = 0; j < 3; ++j) op.params.push_back(std::stod(argv[i++]));
        } 
        else if (op_name == "composite") {
            if (i + 2 >= argc - 1) { std::cerr << "composite requires path, x, y, [opacity]\n"; return 1; }
            op.str_param = argv[i++];
            op.params.push_back(std::stod(argv[i++])); // x
            op.params.push_back(std::stod(argv[i++])); // y
            if (i < argc - 1 && is_number(argv[i])) op.opacity = std::stod(argv[i++]);
        } 
        else if (op_name == "flip" || op_name == "flop" || op_name == "trim" ||
                 op_name == "addalpha" || op_name == "removealpha") {
            // no parameters needed
        } 
        else {
            std::cerr << "Unknown operation: " << op_name << "\n";
            return 1;
        }

        operations.push_back(op);
    }


    PixonImage* img = pixon_create_from_file(input_path.c_str());
    if (!img) { std::cerr << "Failed to load input image: " << input_path << "\n"; return 1; }


    for (const auto& op : operations) {
        OperationC c_op = to_c_operation(op);
        pixon_apply_operation(img, &c_op);
        std::cout << "Applied operation: " << op.name << "\n";
    }

   
    std::string ext = output_path.substr(output_path.find_last_of('.') + 1);
    if (pixon_to_file(img, output_path.c_str(), ext.c_str(), 90) != 0) {
        std::cerr << "Failed to save output image: " << output_path << "\n";
        pixon_destroy(img);
        return 1;
    }

    pixon_destroy(img);
    std::cout << "All operations applied successfully, saved to " << output_path << "\n";
    return 0;
}
