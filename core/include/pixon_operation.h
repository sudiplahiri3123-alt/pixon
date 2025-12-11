#pragma once
#include <string>
#include <vector>

struct PixonImage;


enum class OperationType {
    Resize,
    Rotate,
    Crop,
    ConvertFormat,
    Compress,
    Blur,
    Sharpen,
    Flip,
    Flop,
    Composite,
    ColorAdjust,
    AddAlpha,
    RemoveAlpha,
    Extend,
    Trim
};


struct Operation {
    OperationType type;
    std::vector<double> params;  
    std::string str_param;        
    double opacity = 1.0;        
};


struct OperationC {
    const char* name;   
    int x = 0;          
    int y = 0;          
    int width = 0;      
    int height = 0;     
    double param1 = 0;  
    double param2 = 0;  
    double param3 = 0;  
    const char* path = nullptr; 
};


inline OperationC to_c(const Operation& op) {
    OperationC c{};
    switch(op.type) {
        case OperationType::Resize:
            c.name = "Resize"; 
            if(op.params.size() >= 2) { c.width = (int)op.params[0]; c.height = (int)op.params[1]; }
            break;
        case OperationType::Rotate:
            c.name = "Rotate"; 
            if(!op.params.empty()) c.param1 = op.params[0]; 
            break;
        case OperationType::Crop:
            c.name = "Crop"; 
            if(op.params.size() >= 4) { c.x=(int)op.params[0]; c.y=(int)op.params[1]; c.width=(int)op.params[2]; c.height=(int)op.params[3]; }
            break;
        case OperationType::Blur:
            c.name = "Blur"; 
            if(!op.params.empty()) c.param1 = op.params[0]; 
            break;
        case OperationType::Sharpen:
            c.name = "Sharpen"; 
            if(!op.params.empty()) c.param1 = op.params[0]; 
            break;
        case OperationType::Flip:
            c.name = "Flip"; break;
        case OperationType::Flop:
            c.name = "Flop"; break;
        case OperationType::Composite:
            c.name = "Composite"; 
            if(op.params.size() >= 2) { c.x=(int)op.params[0]; c.y=(int)op.params[1]; } 
            c.path = op.str_param.c_str(); 
            c.param1 = op.opacity;
            break;
        case OperationType::ColorAdjust:
            c.name = "ColorAdjust"; 
            if(op.params.size() >= 3) { c.param1=op.params[0]; c.param2=op.params[1]; c.param3=op.params[2]; }
            break;
        case OperationType::AddAlpha:
            c.name = "AddAlpha"; break;
        case OperationType::RemoveAlpha:
            c.name = "RemoveAlpha"; break;
        case OperationType::Extend:
            c.name = "Extend"; 
            if(op.params.size() >= 4) { c.x=(int)op.params[0]; c.y=(int)op.params[1]; c.width=(int)op.params[2]; c.height=(int)op.params[3]; }
            break;
        case OperationType::Trim:
            c.name = "Trim"; break;
        default:
            c.name = "Unknown"; break;
    }
    return c;
}
