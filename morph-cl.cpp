/**
 * OpenCV morphology with OpenCL tester
 * Copyright (C) 2015  Maciej Borzecki
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <iostream>
#include <cstdio>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

class execution_timer
{
public:
    execution_timer(const string &message)
        : m_message(message) {
        m_start_time = chrono::steady_clock::now();
    }

    ~execution_timer() {
        auto diff = chrono::steady_clock::now() - m_start_time;

        double elapsed_seconds = (double)chrono::duration_cast<chrono::microseconds>(diff).count() / 1e6;

        cerr << "TIME: "
             << m_message
             << ": "
             << elapsed_seconds
             << " s" << endl;
    }

private:
    chrono::steady_clock::time_point m_start_time;
    string m_message;
};


int main(int argc, char *argv[])
{

    bool use_opencl;
    string input_file;
    unsigned int iteration_count;
    unsigned int kernel_size;
    po::options_description desc("Options");

    desc.add_options()
        ("help,h", "Show help")
        ("use-opencl", po::value<bool>(&use_opencl)->default_value(true), "Use OpenCL")
        ("input-file,i", po::value<string>(&input_file), "Input file")
        ("kernel-size,k", po::value<unsigned int>(&kernel_size)->default_value(16), "Kernel size")
        ("iterations", po::value<unsigned int>(&iteration_count)->default_value(10), "Iteration count")
        ;

    po::variables_map opts;
    try {
        po::store(po::parse_command_line(argc, argv, desc), opts);
        po::notify(opts);
    } catch (exception &e) {
        cerr << "failed to parse command line: " << e.what() << endl;
        return -1;
    }

    if (opts.count("help")) {
        cerr << desc << endl;
        return 0;
    }

    if (opts.count("input-file") == 0) {
        cerr << "Missing input file, see --help" << endl;
        return -1;
    }

    cv::ocl::setUseOpenCL(use_opencl);

    if (cv::ocl::useOpenCL())
        cerr << "OpenCL in use" << endl;
    else
        cerr << "OpenCL not used" << endl;

    if (cv::ocl::haveOpenCL()) {
        cerr << "OpenCL available" << endl;

        vector<cv::ocl::PlatformInfo> info;
        cv::ocl::getPlatfomsInfo(info);

        for (auto p : info) {
            cerr << "device: "
                 << p.name()
                 << ":"
                 << p.deviceNumber()
                 << endl;
        }
    }
    else
        cerr << "OpenCL not available" << endl;

    cerr << "input file: " << input_file << endl;

    cv::Mat img = cv::imread(input_file.c_str());
    cv::UMat uimg = img.getUMat(cv::ACCESS_READ);

    if (img.empty()) {
        cerr << "failed to load image"
             << endl;
        return -1;
    }

    cerr << "kernel size: "
         << kernel_size
         << endl;
    int dilation_type = cv::MORPH_ELLIPSE;
    int dilation_size = kernel_size;
    cv::Mat element = cv::getStructuringElement(dilation_type,
                                                cv::Size(dilation_size,
                                                         dilation_size));

    cv::imwrite("kernel.png", element.mul(255));

    cv::UMat uelement = element.getUMat(cv::ACCESS_READ);

    cv::UMat out;

    cerr << "performing "
         << iteration_count
         << " iterations"
         << endl;

    {
        execution_timer for_loop("for");
        for (int i = 0; i < iteration_count; i++)
        {
            // execution_timer("single op");
            cv::dilate(img, out, element);
        }
    }

    cv::imwrite("dilated.png", out);

    return 0;
}
