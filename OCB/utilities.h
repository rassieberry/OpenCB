#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H


#include <vector>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <wtypes.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

namespace ocb
{

    constexpr int IMG_DIVISIONS = 7;
    constexpr int NUMBER_OF_ACTIONS = 60;
    constexpr double MATH_PI = 3.1415926535;
    constexpr int MAX_LOADSTRING = 100;
    constexpr int MAX_FILEPATH = 150;
    constexpr int MAX_PLANS = 8;
    constexpr int MAX_COLOR = 255;
    constexpr int EXISTING_SUBTHREAD = 1;
    constexpr int IMAGINING_SUBTHREAD = 2;
    constexpr int THINKING_SUBTHREAD = 3;


    class Time
    {
    public:
        Time()
        {
            unsigned __int64 beginning = get_current_time();
            std::ofstream record_file;
            record_file.open("birthday.txt.");
            record_file << std::to_string(beginning);
            record_file.close();
        }

        unsigned __int64 get_current_time()
        {
            unsigned __int64 current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            return current_time;
        }

        static long long int get_time()
        {
            unsigned __int64 current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            return current_time;
        }
    };


    class Vision
    {
    public:
        int x = 0;
        int y = 0;
        int h = 256;
        int w = 256;
        int web_x = 0;
        int web_y = 0;
        int web_h = 256;
        int web_w = 256;

        void move_right()
        {
            x = x + 50;
        }

        void move_left()
        {
            x = x - 50;
        }

        void move_up()
        {
            y = y + 50;
        }

        void move_down()
        {
            y = y - 50;
        }

        void zoom_in()
        {
            if (h > 500)
            {
                h = int(h / 2);
                w = int(w / 2);
            }
        }

        void zoom_out()
        {
            if (h < 1000)
            {
                h = h * 2;
                w = w * 2;
            }
        }
        void webcam_move_right()
        {
            x = x + 50;
        }

        void webcam_move_left()
        {
            x = x - 50;
        }

        void webcam_move_up()
        {
            y = y + 50;
        }

        void webcam_move_down()
        {
            y = y - 50;
        }

        void webcam_zoom_in()
        {
            if (h > 260)
            {
                h = int(h / 2);
                w = int(w / 2);
            }
        }

        void webcam_zoom_out()
        {
            if (h < 500)
            {
                h = h * 2;
                w = w * 2;
            }
        }

        cv::Mat take_screenshot()
        {
            HWND hwnd = GetDesktopWindow();
            cv::Mat screenshot = captureScreenMat(hwnd);
            return screenshot;
        }

        cv::Mat capture_webcam()
        {
            int index = 0;
            cv::Mat screenshot;
            cv::VideoCapture webcam = cv::VideoCapture(index);
            webcam.open(index);
            if (webcam.isOpened())
            {
                webcam.read(screenshot);
                webcam.release();
                cv::Rect_ <int> roi = cv::Rect_ <int>(cv::Point(web_x, web_y), cv::Point(web_x + web_w, web_y + web_h));
                cv::Mat crop = screenshot(roi);
                return crop;
            }
            else
            {
                std::cout << "Webcam at index ";
                std::cout << index;
                std::cout << " could not be opened.\n";
                return screenshot;
            }
        }

    private:

        BITMAPINFOHEADER createBitmapHeader(int width, int height)
        {
            BITMAPINFOHEADER  bi;

            bi.biSize = sizeof(BITMAPINFOHEADER);
            bi.biWidth = width;
            bi.biHeight = -height;
            bi.biPlanes = 1;
            bi.biBitCount = 32;
            bi.biCompression = BI_RGB;
            bi.biSizeImage = 0;
            bi.biXPelsPerMeter = 0;
            bi.biYPelsPerMeter = 0;
            bi.biClrUsed = 0;
            bi.biClrImportant = 0;

            return bi;
        }

        cv::Mat captureScreenMat(HWND hwnd)
        {
            cv::Mat screenshot;

            HDC hwindowDC = GetDC(hwnd);
            HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
            SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

            RECT windowsize;
            GetClientRect(hwnd, &windowsize);

            int screenx = x;
            int screeny = y;
            int width = w;
            int height = h;

            screenshot.create(height, width, CV_8UC4);

            HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
            BITMAPINFOHEADER bi = createBitmapHeader(width, height);

            SelectObject(hwindowCompatibleDC, hbwindow);

            StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
            GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

            DeleteObject(hbwindow);
            DeleteDC(hwindowCompatibleDC);
            ReleaseDC(hwnd, hwindowDC);

            return screenshot;
        }
    };


    class Image
    {
    public:
        //	takes an opencv image matrix and converts into a much smaller image for the system to use
        // does this severals times for subsequently larger images
        // only works for grayscale images
        static std::vector<cv::Mat> process_image(cv::Mat img)
        {
            int side = 2;
            side = pow(side, IMG_DIVISIONS);
            std::vector<cv::Mat> img_vector = {};

            int pixels = img.rows * img.cols;
            int remaining_x = img.rows;
            cv::Mat img_layer = cv::Mat(side, side, CV_8UC1, cv::Scalar(255));
            int x = 0;
            for (int i = 0; i < side; i++)
            {
                int xlen = int(remaining_x / (side - i) + 0.5);
                int y = 0;
                int remaining_y = img.cols;
                for (int j = 0; j < side; j++)
                {
                    int ylen = int(remaining_y / (side - j) + 0.5);

                    //cv::Mat mask = cv::Mat(img.rows, img.cols, CV_8UC1, cv::Scalar(255));
                    cv::Rect_ <int> roi = cv::Rect_ <int>(cv::Point(x, y), cv::Point(x + xlen, y + ylen));
                    cv::Mat cropped_img = img(roi);
                    cv::Scalar colors = cv::mean(cropped_img);
                    img_layer.at<char>(cv::Point(i, j)) = colors[0];

                    remaining_y = remaining_y - ylen;
                    y = y + ylen;

                }
                remaining_x = remaining_x - xlen;
                x = x + xlen;

            }


            img_vector.push_back(img_layer);

            int count = side;
            for (int m = 1; m < IMG_DIVISIONS; m++)
            {
                count = int(count / 2);
                pixels = img_vector.at(m - 1).rows * img_vector.at(m - 1).cols;
                remaining_x = img_vector.at(m - 1).rows;
                cv::Mat img_layer2 = cv::Mat(count, count, CV_8UC1, cv::Scalar(255));
                int x = 0;
                for (int j = 0; j < count; j++)
                {
                    int y = 0;
                    int xlen = int(remaining_x / (count - j) + 0.5);
                    int remaining_y = img_vector.at(m - 1).cols;
                    for (int k = 0; k < count; k++)
                    {
                        int ylen = int(remaining_y / (count - k) + 0.5);

                        //cv::Mat mask = cv::Mat(img_vector.at(m-1).rows, img_vector.at(m-1).cols, CV_8UC1, cv::Scalar(255));
                        cv::Rect_ <int> roi = cv::Rect_ <int>(cv::Point(x, y), cv::Point(x + xlen, y + ylen));
                        cv::Mat cropped_img = img_vector.at(m - 1)(roi);
                        cv::Scalar colors = cv::mean(cropped_img);
                        img_layer2.at<char>(cv::Point(j, k)) = colors[0];

                        remaining_y = remaining_y - ylen;
                        y = y + ylen;
                    }
                    remaining_x = remaining_x - xlen;
                    x = x + xlen;
                }
                img_vector.push_back(img_layer2);



            }
            return img_vector;
        }


        static cv::Mat grayscale_file(std::string image_path)
        {
            cv::Mat img = cv::imread(image_path);
            cv::cvtColor(img, img, cv::COLOR_RGBA2GRAY, 1);
            return img;
        }

        static cv::Mat grayscale()
        {
            cv::Mat img;
            cv::Mat img_gray;
            std::string path = "sight1.jpg";
            std::fstream fs;
            fs.open(path);
            if (fs.is_open())
            {
                fs.close();
                img = cv::imread(path);
                cv::cvtColor(img, img_gray, cv::COLOR_RGBA2GRAY, 1);

            }
            return img_gray;
        }

        static cv::Mat grayscale_webcam()
        {
            cv::Mat img;
            cv::Mat img_gray;
            std::string path = "sight2.jpg";
            std::fstream fs;
            fs.open(path);
            if (fs.is_open())
            {
                fs.close();
                img = cv::imread(path);
                cv::cvtColor(img, img_gray, cv::COLOR_RGBA2GRAY, 1);

            }
            return img_gray;
        }


    };



}
#endif // UTILITIES_H
