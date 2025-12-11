#pragma once
#include <cstddef>  // for size_t

#ifdef __cplusplus
extern "C" {
#endif


struct PixonImage;


struct OperationC {
    const char* name;       
    int x, y, width, height; 
    double param1, param2, param3; 
    const char* path;       
};


PixonImage* pixon_create_from_file(const char* path);
PixonImage* pixon_create_from_buffer(const void* data, size_t len);
void pixon_destroy(PixonImage* img);


int pixon_to_file(PixonImage* img, const char* path, const char* format, int quality);
unsigned char* pixon_to_buffer(PixonImage* img, size_t* sz, const char* format, int quality);


void pixon_apply_operation(PixonImage* img, const OperationC* op);

#ifdef __cplusplus
}
#endif
