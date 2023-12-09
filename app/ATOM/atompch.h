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

#include "ATOM/Core/Logging/Logging.h"
#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"


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



#endif //ATOM_ATOMPCH_H
