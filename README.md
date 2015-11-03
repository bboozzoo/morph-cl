# morph-cl

Small test tool for testing/benchmarking OpenCV 3.0 morphology operation. 

The code specifically uses `cv::UMat ` types so that OpenCV may attempt to
invoke an OpenCL optimized code path. However, there is no guarantee that the
actual OpenCL implementation will run as that depends on a number of factors
such as library availability or kernel size. There's even no guarantee that
the implementation will perform better than the CPU.

## Usage

Requirements:
* cmake
* OpenCV 3.0
* boost::program_options

Then run:

    mkdir build
    cd build
    cmake ..

The input image is passed with `-i` parameter. For other parameters see the output
of `--help`.
