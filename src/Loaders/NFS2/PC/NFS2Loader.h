#pragma once

#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstdlib>
#include <bitset>
#include "bmpread.h"
#include "../../TrackUtils.h"
#include "../../../Physics/Car.h"
#include "../../../Config.h"
#include "../../../Util/ImageLoader.h"
#include "../../../Util/Utils.h"
#include "../../../Scene/TrackBlock.h"
#include "../../../nfs_data.h"
#include "../../../Util/Logger.h"

#define scaleFactor 1000000.0f

using namespace NFS2_DATA;

template <typename Platform>
class NFS2
{
public:
    static std::shared_ptr<typename Platform::TRACK> LoadTrack(const std::string &track_base_path);
    static std::shared_ptr<Car> LoadCar(const std::string &car_base_path, NFSVer version);

    static void dbgPrintVerts(const std::string &path, const std::shared_ptr<typename Platform::TRACK> &track);

private:
    // Car
    static std::vector<CarModel> LoadGEO(const std::string &geo_path, std::map<unsigned int, Texture> car_textures, std::map<std::string, uint32_t> remapped_texture_ids);
    // Track
    static bool LoadTRK(std::string trk_path, const std::shared_ptr<typename Platform::TRACK> &track);
    static bool LoadCOL(std::string col_path, const std::shared_ptr<typename Platform::TRACK> &track);
    static void ParseTRKModels(const std::shared_ptr<typename Platform::TRACK> &track);
    static std::vector<Entity> ParseCOLModels(const std::shared_ptr<typename Platform::TRACK> &track);
    static Texture LoadTexture(TEXTURE_BLOCK track_texture, const std::string &track_name, NFSVer nfs_version);
};
