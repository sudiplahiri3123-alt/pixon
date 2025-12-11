#include "pixon_operation.h"
#include "pixon_image.h"  

void apply_operation(PixonImage* img, const Operation& op) {
    if (!img) return;

    switch(op.type) {
        case OperationType::Resize:
            img->image.resize(static_cast<int>(op.params[0]),
                              static_cast<int>(op.params[1]),
                              op.params.size() > 2 ? op.params[2] != 0 : false);
            break;
        case OperationType::Rotate:
            img->image.rotate(static_cast<int>(op.params[0]));
            break;
        case OperationType::Crop:
            img->image.crop(
                static_cast<int>(op.params[0]), static_cast<int>(op.params[1]),
                static_cast<int>(op.params[2]), static_cast<int>(op.params[3])
            );
            break;
        case OperationType::Blur:
            img->image.blur(op.params[0]);
            break;
        case OperationType::Sharpen:
            img->image.sharpen(op.params[0]);
            break;
        case OperationType::Flip:
            img->image.flip();
            break;
        case OperationType::Flop:
            img->image.flop();
            break;
        case OperationType::Composite:
            img->image.composite(op.str_param.c_str(),
                                 static_cast<int>(op.params[0]),
                                 static_cast<int>(op.params[1]),
                                 op.opacity);
            break;
        case OperationType::ColorAdjust:
            img->image.adjust(op.params[0], op.params[1], op.params[2]);
            break;
        case OperationType::AddAlpha:
            img->image.addAlpha();
            break;
        case OperationType::RemoveAlpha:
            img->image.removeAlpha();
            break;
        case OperationType::Extend:
            img->image.extend(
                static_cast<int>(op.params[0]),
                static_cast<int>(op.params[1]),
                static_cast<int>(op.params[2]),
                static_cast<int>(op.params[3])
            );
            break;
        case OperationType::Trim:
            img->image.trim();
            break;
        default:
            break;
    }
}
