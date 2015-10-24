#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

class execution_timer
{
public:
    execution_timer(const std::string &message)
        : m_message(message) {
        m_start_time = steady_clock::now();
    }

    ~execution_timer() {
        auto diff = steady_clock::now() - m_start_time;

        double elapsed_seconds = (double)duration_cast<microseconds>(diff).count() / 1e6;

        std::fprintf(stdout, "TIME: %s: %f s\n", m_message.c_str(), elapsed_seconds);
    };

private:
    std::chrono::steady_clock::time_point m_start_time;
    std::string m_message;
};


int main(int argc, char *argv[])
{

    if (argc < 2) {
        std::cerr << "usage: <prog> <image file>"
                  << std::endl;
        return -1;
    }

    cv::Mat img = cv::imread(argv[1]);
    cv::UMat uimg = img.getUMat(cv::ACCESS_READ);

    if (img.empty()) {
        std::cerr << "failed to load image"
                  << std::endl;
        return -1;
    }

    int dilation_type = cv::MORPH_ELLIPSE;
    int dilation_size = 10;
    cv::Mat element = cv::getStructuringElement(dilation_type,
                                                cv::Size(2 * dilation_size + 1,
                                                          2 * dilation_size + 1),
                                                cv::Point(dilation_size,
                                                          dilation_size));
    cv::UMat uelement = element.getUMat(cv::ACCESS_READ);

    cv::UMat out;
    cv::dilate(img, out, element);

    cv::imwrite("dilated.png", out);

    return 0;
}
