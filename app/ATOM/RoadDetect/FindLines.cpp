//
// Created by toor on 5/16/24.
//


#include "FindLines.h"

namespace Atom
{
    std::vector<cv::Point2f> slidingWindowLeft(cv::Mat image, cv::Rect window);

    float calculateOffset(const std::vector<cv::Point2f>& points, const cv::Size& imageSize);

    FindLines::FindLines()
    {
        lineSettings = LineSettings();
        srcVertices[0] = cv::Point(lineSettings.offsetSides, lineSettings.topOffset);
        srcVertices[1] = cv::Point(640 - lineSettings.offsetSides, lineSettings.topOffset);
        srcVertices[2] = cv::Point(640 - lineSettings.bottomoffsetSides, lineSettings.bottomOffset);
        srcVertices[3] = cv::Point(lineSettings.bottomoffsetSides, lineSettings.bottomOffset);


        dstVertices[0] = cv::Point(0, 0);
        dstVertices[1] = cv::Point(640, 0);
        dstVertices[2] = cv::Point(640, 480);
        dstVertices[3] = cv::Point(0, 480);
        perspectiveMatrix = cv::getPerspectiveTransform(srcVertices, dstVertices);
        dst = cv::Mat(480, 640, CV_8UC3);
        cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);

        blackMaskVertives[0] = cv::Point(220, 300);
        blackMaskVertives[1] = cv::Point(440, 300);


        blackMaskVertives[2] = cv::Point(520, 370);
        blackMaskVertives[3] = cv::Point(120, 370);
    }

    FindLines::~FindLines()
    {
    }


    cv::Mat& FindLines::GetPreprocessed()
    {
        return preprocessed;
    }

    float FindLines::PreprocessLine(cv::Mat& org)
    {
        m_PointsOnLine.clear();

        if (lastOffset != (lineSettings.offsetSides + lineSettings.bottomoffsetSides + lineSettings.topOffset +
            lineSettings.bottomOffset + int(lineSettings.leftLineSide * 10)))
        {
            srcVertices[0] = cv::Point(lineSettings.offsetSides, lineSettings.topOffset);
            srcVertices[1] = cv::Point(640 - lineSettings.offsetSides, lineSettings.topOffset);
            srcVertices[2] = cv::Point(640 - lineSettings.bottomoffsetSides, lineSettings.bottomOffset);
            srcVertices[3] = cv::Point(lineSettings.bottomoffsetSides, lineSettings.bottomOffset);


            dstVertices[0] = cv::Point(0, 0);
            dstVertices[1] = cv::Point(640, 0);
            dstVertices[2] = cv::Point(640, 480);
            dstVertices[3] = cv::Point(0, 480);
            perspectiveMatrix = cv::getPerspectiveTransform(srcVertices, dstVertices);
            dst = cv::Mat(480, 640, CV_8UC3);
            cv::invert(perspectiveMatrix, invertedPerspectiveMatrix);
            lastOffset = (lineSettings.offsetSides + lineSettings.bottomoffsetSides + lineSettings.topOffset +
                lineSettings.bottomOffset + int(lineSettings.leftLineSide * 10));
            ATLOG_WARN("Offset Update");
        }

        org.copyTo(original);
        // //blck mask
        // cv::fillConvexPoly(original, blackMaskVertives, 4, cv::Scalar(0, 0, 0));
        if (lineSettings.filledCircle)
        {
            cv::circle(original, {320, lineSettings.circlePosition}, lineSettings.circleSize, cv::Scalar(0, 0, 0, 150),
                       cv::FILLED);
        }
        else
        {
            cv::circle(original, {320, lineSettings.circlePosition}, lineSettings.circleSize, cv::Scalar(0, 0, 0, 150),
                       8);
        }


        cv::warpPerspective(original, dst, perspectiveMatrix, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
        cv::cvtColor(dst, img, cv::COLOR_RGB2GRAY);
        cv::inRange(img, lineSettings.lowerWhite, lineSettings.upperWhite, whiteMask);
        whiteMask.copyTo(processed);
        const cv::Size kernelSize = cv::Size(9, 9);
        cv::GaussianBlur(processed, processed, kernelSize, 0);
        cv::Mat kernel = cv::Mat::ones(15, 15, CV_8U);
        cv::dilate(processed, processed, kernel);
        cv::erode(processed, processed, kernel);
        cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel);
        const int thresholdVal = 120;
        cv::threshold(processed, processed, thresholdVal, 255, cv::THRESH_BINARY);
        //
        //
        //        cv::GaussianBlur(processed, processed, cv::Size(5, 5), 0);
        //        cv::Canny(processed, processed, 50, 150);

        std::vector<cv::Point2f> pts = slidingWindow(processed, cv::Rect(0, 450, RectSize.x, RectSize.y));
        cv::rectangle(original, cv::Rect(0, 450, RectSize.x, RectSize.y), cv::Scalar(0, 255, 0), 2);
        std::vector<cv::Point> allPts;
        std::vector<cv::Point2f> outPts;
        cv::perspectiveTransform(pts, outPts, invertedPerspectiveMatrix);
        leftLanePts = outPts;
        for (unsigned int i = 0; i < outPts.size() - 1; ++i)
        {
            line(original, outPts[i], outPts[i + 1], cv::Scalar(255, 0, 0), 3);
            allPts.push_back(cv::Point(outPts[i].x, outPts[i].y));
        }
        allPts.push_back(cv::Point(outPts[outPts.size() - 1].x, outPts[outPts.size() - 1].y));

        pts = slidingWindow(processed, cv::Rect(550, 450, RectSize.x, RectSize.y));
        cv::rectangle(original, cv::Rect(550, 450, RectSize.x, RectSize.y), cv::Scalar(0, 255, 0), 2);
        perspectiveTransform(pts, outPts, invertedPerspectiveMatrix);


        rightLanePts = outPts;
        for (unsigned int i = 0; i < outPts.size() - 1; ++i)
        {
            line(original, outPts[i], outPts[i + 1], cv::Scalar(0, 0, 255), 3);
            allPts.push_back(cv::Point(outPts[outPts.size() - i - 1].x, outPts[outPts.size() - i - 1].y));
        }
        allPts.push_back(cv::Point(outPts[0].x - (outPts.size() - 1), outPts[0].y));
        std::vector<std::vector<cv::Point>> arr;

        arr.push_back(allPts);
        overlay = cv::Mat::zeros(original.size(), original.type());
        cv::fillPoly(overlay, arr, cv::Scalar(0, 255, 100));
        cv::addWeighted(original, 1, overlay, 0.5, 0, original);


        cv::circle(original, srcVertices[0], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[1], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[2], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        cv::circle(original, srcVertices[3], 10, cv::Scalar(25, 0, 255), cv::FILLED);
        float leftOffset = calculateOffset(leftLanePts, org.size());
        float rightOffset = calculateOffset(rightLanePts, org.size());
        laneCenterOffset = (leftOffset + rightOffset) / 2.0f;


        //using perspective matrix to get the points back to the original image
        //m_PointsOnLine
        cv::perspectiveTransform(m_PointsOnLine, m_PointsOnLine, invertedPerspectiveMatrix);

        m_PointsOnLineTransformed.clear();
        for (unsigned int i = 0; i < m_PointsOnLine.size(); ++i)
        {
            m_PointsOnLineTransformed.push_back(cv::Point2i(m_PointsOnLine[i].x, m_PointsOnLine[i].y));
        }

        return laneCenterOffset;
    }


    std::vector<cv::Point2f> FindLines::slidingWindow(cv::Mat image, cv::Rect window)
    {
        std::vector<cv::Point2f> points;
        const cv::Size imgSize = image.size();
        bool shouldBreak = false;
        while (true)
        {
            float currentX = window.x + window.width * lineSettings.leftLineSide;
            cv::Mat roi = image(window);
            std::vector<cv::Point2f> locations;
            findNonZero(roi, locations);
            float avgX = 0.0f;
            for (unsigned int i = 0; i < locations.size(); ++i)
            {
                float x = locations[i].x;
                avgX += window.x + x;
                //draw all the points
            }
            avgX = locations.empty() ? currentX : avgX / locations.size();

            cv::Point point(avgX, window.y + window.height * 0.1f);
            points.push_back(point);


            m_PointsOnLine.push_back(point);

            window.y -= window.height;
            if (window.y < 0)
            {
                window.y = 0;
                shouldBreak = true;
            }
            window.x += (point.x - currentX);
            if (window.x < 0)
                window.x = 0;
            if (window.x + window.width >= imgSize.width)
                window.x = imgSize.width - window.width - 1;
            if (shouldBreak)
                break;

            //draw the sliding rect in red color
            cv::rectangle(original, window, cv::Scalar(0, 0, 255), 2);
            //            this thread sleep for 30 ms
            //            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return points;
    }

    float calculateOffset(const std::vector<cv::Point2f>& points, const cv::Size& imageSize)
    {
        float centerX = static_cast<float>(imageSize.width) / 2.0f;
        float sumOffset = 0.0f;
        for (const cv::Point2f& point : points)
        {
            float pointX = point.x;
            float offset = pointX - centerX;
            sumOffset += offset;
        }
        float averageOffset = sumOffset / points.size();
        return averageOffset;
    }
}
