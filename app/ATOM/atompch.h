//
// Created by toor on 11/14/23.
//

#ifndef ATOM_ATOMPCH_H
#define ATOM_ATOMPCH_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#include <map>
#include <thread>
#include <functional>
#include "glm/glm.hpp"
#include "ATOM/Core/Logging/Logging.h"
#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"

#include "opencv2/opencv.hpp"


#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#ifdef _WIN32
#include <windows.h> // Ug, for NukeProcess -- see below
#else
#include <unistd.h>
#include <signal.h>
#endif

struct LineSettings {
    int circleSize = 168;
    int circlePosition = 384;
    bool filledCircle = true;

    int offsetSides = 172;
    int topOffset = 144;
    int bottomoffsetSides = 120;
    int bottomOffset = 300789;
    float  leftLineSide = 0.5;
    cv::Scalar lowerWhite = cv::Scalar(190,200,200);
    cv::Scalar upperWhite = cv::Scalar(255, 255, 255);
    bool showDebug = true;
};

#endif //ATOM_ATOMPCH_H
