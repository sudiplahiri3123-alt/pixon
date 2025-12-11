#include "pixon_c_api.h"
#include <stdexcept>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

struct ImageProcessor {
    cv::Mat img;

    ImageProcessor() = default;

    ImageProcessor(const char* path) {
        img = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (img.empty()) throw std::runtime_error("Failed to load image: " + std::string(path));
    }

    void fromBuffer(const void* data, size_t len) {
        std::vector<uchar> buf((uchar*)data, (uchar*)data + len);
        img = cv::imdecode(buf, cv::IMREAD_UNCHANGED);
        if (img.empty()) throw std::runtime_error("Failed to decode buffer");
    }

    void convert(const char* path, int quality = 90) {
        std::vector<int> params;
        std::string ext(path);
        if (ext.find(".jpg") != std::string::npos || ext.find(".jpeg") != std::string::npos)
            params = {cv::IMWRITE_JPEG_QUALITY, quality};
        else if (ext.find(".png") != std::string::npos)
            params = {cv::IMWRITE_PNG_COMPRESSION, 3};
        else if (ext.find(".webp") != std::string::npos)
            params = {cv::IMWRITE_WEBP_QUALITY, quality};
#if defined(IMWRITE_AVIF_QUALITY)
        else if (ext.find(".avif") != std::string::npos)
            params = {cv::IMWRITE_AVIF_QUALITY, quality};
#endif
#if defined(IMWRITE_GIF_LOOP)
        else if (ext.find(".gif") != std::string::npos)
            params = {cv::IMWRITE_GIF_LOOP, 0};
#endif
        else throw std::runtime_error("Unsupported output format: " + ext);
        if (!cv::imwrite(path, img, params)) throw std::runtime_error("Failed to save image");
    }

    std::vector<uint8_t> toBuffer(const char* format, int quality) {
        std::vector<uint8_t> buf;
        std::vector<int> params;
        std::string fmt(format);
        if (fmt == ".jpg" || fmt == ".jpeg") params = {cv::IMWRITE_JPEG_QUALITY, quality};
        else if (fmt == ".png") params = {cv::IMWRITE_PNG_COMPRESSION, 3};
        else if (fmt == ".webp") params = {cv::IMWRITE_WEBP_QUALITY, quality};
#if defined(IMWRITE_AVIF_QUALITY)
        else if (fmt == ".avif") params = {cv::IMWRITE_AVIF_QUALITY, quality};
#endif
#if defined(IMWRITE_GIF_LOOP)
        else if (fmt == ".gif") params = {cv::IMWRITE_GIF_LOOP, 0};
#endif
        else throw std::runtime_error("Unsupported buffer format: " + fmt);
        if (!cv::imencode(fmt, img, buf, params)) throw std::runtime_error("Failed to encode image");
        return buf;
    }

   
    void resize(int w, int h, bool crop) {
        if (crop) {
            int x = std::max(0, (img.cols - w) / 2);
            int y = std::max(0, (img.rows - h) / 2);
            cv::Rect roi(x, y, std::min(w, img.cols - x), std::min(h, img.rows - y));
            img = img(roi);
        }
        cv::resize(img, img, cv::Size(w, h));
    }

    void rotate(int angle) {
        if (angle % 360 == 0) return;
        cv::Mat dst;
        cv::Point2f center(img.cols / 2.0F, img.rows / 2.0F);
        cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::warpAffine(img, dst, rot, img.size());
        img = dst;
    }

    void crop(int x, int y, int w, int h) { 
        img = img(cv::Rect(x, y, std::min(w, img.cols - x), std::min(h, img.rows - y))); 
    }

    void flip() { cv::flip(img, img, 0); }
    void flop() { cv::flip(img, img, 1); }
    void blur(double sigma) { cv::GaussianBlur(img, img, cv::Size(0, 0), sigma); }
    void sharpen(double sigma = 1.0) {
    if (sigma <= 0) sigma = 1.0;
    cv::Mat blurred;
    cv::GaussianBlur(img, blurred, cv::Size(0,0), sigma);
    cv::addWeighted(img, 1.5, blurred, -0.5, 0, img);
    }

    void adjust(double brightness, double contrast, double saturation) {
        img.convertTo(img, -1, contrast, brightness * 255);
        if (img.channels() >= 3) {
            cv::Mat hsv; 
            cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
            std::vector<cv::Mat> ch; 
            cv::split(hsv, ch); 
            ch[1] *= saturation;
            cv::merge(ch, hsv); 
            cv::cvtColor(hsv, img, cv::COLOR_HSV2BGR);
        }
    }

    void composite(const char* overlayPath, int left, int top, double opacity = 1.0) {
        cv::Mat overlay = cv::imread(overlayPath, cv::IMREAD_UNCHANGED);
        if (overlay.empty()) return;
        int w = std::min(overlay.cols, img.cols - left);
        int h = std::min(overlay.rows, img.rows - top);
        if (w <= 0 || h <= 0) return;
        cv::Mat roi = img(cv::Rect(left, top, w, h));
        cv::Mat overlayRoi = overlay(cv::Rect(0, 0, w, h));
        cv::addWeighted(overlayRoi, opacity, roi, 1.0 - opacity, 0, roi);
    }

    void addAlpha() { if (img.channels() == 3) cv::cvtColor(img, img, cv::COLOR_BGR2BGRA); }
    void removeAlpha() { if (img.channels() == 4) cv::cvtColor(img, img, cv::COLOR_BGRA2BGR); }

    // --------------------
    // Robust extend
    // --------------------
    void extend(int top, int bottom, int left, int right, const cv::Scalar& color = cv::Scalar(0, 0, 0, 0)) {
        if (img.channels() == 1)
            cv::copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(color[0]));
        else if (img.channels() == 3)
            cv::copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT,
                               cv::Scalar(color[0], color[1], color[2]));
        else if (img.channels() == 4)
            cv::copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT, color);
        else
            throw std::runtime_error("Unsupported number of channels in extend");
    }

  
    void trim() {
        if (img.empty()) return;
        int channels = img.channels();
        int top = 0, bottom = img.rows - 1, left = 0, right = img.cols - 1;

        auto isRowEmpty = [&](int y) -> bool {
            for (int x = 0; x < img.cols; ++x) {
                if (channels == 1 && img.at<uchar>(y, x) != 0) return false;
                else if (channels == 3) {
                    cv::Vec3b px = img.at<cv::Vec3b>(y, x);
                    if (px[0] != 0 || px[1] != 0 || px[2] != 0) return false;
                } else if (channels == 4) {
                    cv::Vec4b px = img.at<cv::Vec4b>(y, x);
                    if (px[3] != 0) return false;
                }
            }
            return true;
        };

        auto isColEmpty = [&](int x) -> bool {
            for (int y = 0; y < img.rows; ++y) {
                if (channels == 1 && img.at<uchar>(y, x) != 0) return false;
                else if (channels == 3) {
                    cv::Vec3b px = img.at<cv::Vec3b>(y, x);
                    if (px[0] != 0 || px[1] != 0 || px[2] != 0) return false;
                } else if (channels == 4) {
                    cv::Vec4b px = img.at<cv::Vec4b>(y, x);
                    if (px[3] != 0) return false;
                }
            }
            return true;
        };

        for (; top <= bottom; ++top) if (!isRowEmpty(top)) break;
        for (; bottom >= top; --bottom) if (!isRowEmpty(bottom)) break;
        for (; left <= right; ++left) if (!isColEmpty(left)) break;
        for (; right >= left; --right) if (!isColEmpty(right)) break;

        if (top <= bottom && left <= right)
            img = img(cv::Rect(left, top, right - left + 1, bottom - top + 1));
    }
};

struct PixonImage { ImageProcessor image; };

#define CATCH_ERRORS(retval) catch(const std::exception& e){fprintf(stderr,"Pixon Error: %s\n",e.what());return retval;}

extern "C" {

PixonImage* pixon_create_from_file(const char* path) { try { return new PixonImage{ImageProcessor(path)}; } CATCH_ERRORS(nullptr) }
PixonImage* pixon_create_from_buffer(const void* data, size_t len) { try { PixonImage* img=new PixonImage{}; img->image.fromBuffer(data,len); return img; } CATCH_ERRORS(nullptr) }
void pixon_destroy(PixonImage* img) { delete img; }

int pixon_to_file(PixonImage* img,const char* path,const char* format,int quality) { try { img->image.convert(path,quality); return 0; } CATCH_ERRORS(-1) }
unsigned char* pixon_to_buffer(PixonImage* img,size_t* sz,const char* format,int quality) {
    try { std::vector<uint8_t> buf = img->image.toBuffer(format,quality); unsigned char* out=new unsigned char[buf.size()]; std::memcpy(out,buf.data(),buf.size()); *sz=buf.size(); return out; } CATCH_ERRORS(nullptr)
}

void pixon_apply_operation(PixonImage* img,const OperationC* op) {
    try {
        std::string name(op->name);
        if(name=="resize") img->image.resize(op->width,op->height,op->param3!=0.0);
        else if(name=="rotate") img->image.rotate((int)op->param1);
        else if(name=="crop") img->image.crop(op->x,op->y,op->width,op->height);
        else if(name=="blur") img->image.blur(op->param1);
        else if(name=="sharpen") img->image.sharpen(op->param1);
        else if(name=="flip") img->image.flip();
        else if(name=="flop") img->image.flop();
        else if(name=="composite") img->image.composite(op->path,op->x,op->y,op->param1);
        else if(name=="coloradjust") img->image.adjust(op->param1,op->param2,op->param3);
        else if(name=="addalpha") img->image.addAlpha();
        else if(name=="removealpha") img->image.removeAlpha();
        else if(name=="extend") img->image.extend(op->y,op->height,op->x,op->width);
        else if(name=="trim") img->image.trim();
    } CATCH_ERRORS()
}

} // extern "C"
