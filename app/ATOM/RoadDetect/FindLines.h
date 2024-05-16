//
// Created by toor on 5/16/24.
//

#ifndef FINDLINES_H
#define FINDLINES_H


#include "ATOM/atompch.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "ATOM/Core/ServerLayer.h"
#include "ATOM/Core/VideoServer/Frame.h"
#include "Networking/Server.h"


namespace Atom
{
    class FindLines
    {
    public:
        FindLines();
        ~FindLines();


        cv::Mat& GetPreprocessed();
        LineSettings& GetLineSettings() { return lineSettings; }
        glm::vec<2, int>& GetRectSize() { return RectSize; }
        float PreprocessLine(cv::Mat& original);
        cv::Mat& GetProcessed() { return processed; }
        void SetLineSettings(LineSettings settings) { lineSettings = settings; }
        std::vector<cv::Point2i> GetLinePoints(){return m_PointsOnLineTransformed;}

    private:
        std::vector<cv::Point2f> slidingWindow(cv::Mat image, cv::Rect window);
    private:
        std::vector<cv::Point2f> m_PointsOnLine;
        std::vector<cv::Point2i> m_PointsOnLineTransformed;

        glm::vec<2, int> RectSize = glm::vec2(90, 30);
        cv::Mat original;
        glm::vec2 frameSize = glm::vec2(640, 480);
        cv::Mat preprocessed;
        float laneCenterOffset = 0.0f;
        cv::Point2f srcVertices[4];
        cv::Point2f dstVertices[4];
        cv::Mat perspectiveMatrix;
        cv::Mat dst;
        cv::Mat invertedPerspectiveMatrix;
        cv::Mat whiteMask;
        cv::Mat processed;
        cv::Mat img;
        cv::Mat out;
        cv::Mat overlay;
        LineSettings lineSettings;
        std::vector<cv::Point2f> leftLanePts;
        std::vector<cv::Point2f> rightLanePts;
        int lastOffset = 1;

        float m_WidthMultiplier = 1.25;

        cv::Point blackMaskVertives[4];
    };
}


#endif //FINDLINES_H
