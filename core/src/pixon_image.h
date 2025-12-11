#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

struct ImageProcessor {
    cv::Mat img;

    ImageProcessor() = default;
    ImageProcessor(const char* path) {
        img = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (img.empty()) throw std::runtime_error("Failed to load image");
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
        else throw std::runtime_error("Unsupported output format: " + ext);
        cv::imwrite(path, img, params);
    }

    std::vector<uint8_t> toBuffer(const char* format, int quality = 90) {
        std::vector<uint8_t> buf;
        std::vector<int> params;
        std::string fmt(format);
        if (fmt == ".jpg" || fmt == ".jpeg") params = {cv::IMWRITE_JPEG_QUALITY, quality};
        else if (fmt == ".png") params = {cv::IMWRITE_PNG_COMPRESSION, 3};
        else throw std::runtime_error("Unsupported buffer format: " + fmt);
        if (!cv::imencode(fmt, img, buf, params))
            throw std::runtime_error("Failed to encode image to buffer");
        return buf;
    }

    // Core operations
    void resize(int w, int h, bool crop = false);
    void rotate(int angle);
    void crop(int x, int y, int w, int h);
    void flip();
    void flop();
    void blur(double sigma);
    void sharpen(double sigma);
    void adjust(double brightness, double contrast, double saturation);
    void composite(const char* overlayPath, int left, int top, double opacity=1.0);
    void addAlpha();
    void removeAlpha();
    void extend(int top, int bottom, int left, int right, const cv::Scalar& color = cv::Scalar(0,0,0,0));
    void trim();
};

struct PixonImage {
    ImageProcessor image;
};
