#pragma once

#include "stdint.h"
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <cstring>
#include <boost/filesystem.hpp>
#include "../../TrackUtils.h"
#include "SerializedGroupOps.h"
#include "../../../Util/Logger.h"
#include "../../../Physics/Car.h"
#include "../../../Util/ImageLoader.h"
#include "../../../Util/Utils.h"
#include "../../../nfs_data.h"

using namespace NFS3_4_DATA::PS1;

// Dumped from IDA, [column 1] % 2 is used to check for part normal presence
static constexpr uint8_t R3DCar_ObjectInfo[57][6] = {
  0x00, 0x49, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x02, 0x01, 0x01, 0x00, 0x00, 0x30, 0x00, 0x01, 0x01, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x01,
  0x00, 0xF0, 0x08, 0x0A, 0x0A, 0x00, 0x00, 0xE0, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x0C, 0x00, 0x00, 0xEC, 0x89, 0x0B, 0x0B, 0x00, 0x0B, 0xF0, 0x88, 0x0B, 0x0B,
  0x00, 0x0B, 0xEC, 0x89, 0x0C, 0x0C, 0x00, 0x0C, 0xF0, 0x88, 0x0C, 0x0C, 0x00, 0x0C, 0xE8, 0x00, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x01, 0x00, 0x00, 0xD4, 0x00, 0x11,
  0x00, 0x00, 0x00, 0xD4, 0x00, 0x11, 0x00, 0x00, 0x00, 0xE1, 0x08, 0x01, 0x00, 0x00, 0x00, 0xE1, 0x08, 0x00, 0x01, 0x00, 0x00, 0xD4, 0x00, 0x12, 0x12, 0x12, 0x00, 0xE2, 0x00,
  0x01, 0x00, 0x00, 0x00, 0xE2, 0x00, 0x00, 0x01, 0x00, 0x00, 0xD4, 0x00, 0x13, 0x13, 0x13, 0x00, 0xE2, 0x18, 0x0F, 0x10, 0x00, 0x00, 0xE2, 0x08, 0x00, 0x01, 0x00, 0x00, 0xD4,
  0x10, 0x14, 0x14, 0x14, 0x00, 0xE2, 0x18, 0x0F, 0x10, 0x00, 0x00, 0xE2, 0x08, 0x00, 0x01, 0x00, 0x00, 0xD4, 0x10, 0x15, 0x15, 0x15, 0x00, 0xE8, 0x08, 0x01, 0x00, 0x00, 0x00,
  0xE8, 0x08, 0x00, 0x01, 0x00, 0x00, 0xD4, 0x00, 0x16, 0x16, 0x16, 0x00, 0xD8, 0x00, 0x01, 0x01, 0x00, 0x00, 0xF4, 0x00, 0x0D, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x0E, 0x00, 0x00,
  0x00, 0xD4, 0x00, 0x11, 0x00, 0x00, 0x00, 0x30, 0x00, 0x02, 0x01, 0x00, 0x00, 0x28, 0x02, 0x03, 0x00, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x03, 0x26, 0x02, 0x04, 0x00,
  0x00, 0x00, 0x24, 0x02, 0x04, 0x00, 0x00, 0x04, 0x24, 0x02, 0x04, 0x00, 0x00, 0x04, 0x00, 0x49, 0x01, 0x00, 0x00, 0x01, 0x00, 0x49, 0x01, 0x00, 0x00, 0x01, 0xF0, 0x80, 0x05,
  0x00, 0x00, 0x05, 0xF0, 0x80, 0x06, 0x00, 0x00, 0x06, 0xE8, 0x89, 0x07, 0x07, 0x00, 0x07, 0xE8, 0x89, 0x08, 0x08, 0x00, 0x08, 0x1F, 0x00, 0x01, 0x01, 0x00, 0x00, 0x1F, 0x00,
  0x01, 0x01, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x09, 0x01, 0x00, 0x00, 0x20, 0x00, 0x09, 0x01, 0x00, 0x00, 0x20,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x09, 0x01, 0x00, 0x00, 0x20, 0x00, 0x09, 0x01, 0x00, 0x00};

// TODO: Get this confirmed against another vehicle
static constexpr char geoPartNames[57][32] = {"Body Medium",
                                              "Body Low",
                                              "Body Undertray",
                                              "Wheel Wells",
                                              "Wheel Squares (unknown LOD)",
                                              "Wheel Squares (A little bigger)",
                                              "Unknown Invisible??",
                                              "Unknown Invisible??",
                                              "Spoiler Original",
                                              "Spoiler Uprights",
                                              "Spoiler Upgraded",
                                              "Spoiler Upgraded Uprights ",
                                              "Fog Lights and Rear bumper Top",
                                              "Fog Lights and Rear bumper TopR",
                                              "Wing Mirror Attachment Points",
                                              "Wheel Attachment Points",
                                              "Brake Light Quads",
                                              "Unknown Invisible",
                                              "Unknown Rear Light tris",
                                              "Rear Inner Light Quads",
                                              "Rear Inner Light Quads rotated",
                                              "Rear Inner Light Tris",
                                              "Front Light quads",
                                              "Bigger Front Light quads",
                                              "Front Light triangles",
                                              "Rear Main Light Quads",
                                              "Rear Main Light Quads dup",
                                              "Rear Main Light Tris",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Front Headlight light Pos",
                                              "Logo and Rear Numberplate",
                                              "Exhaust Tips",
                                              "Low LOD Exhaust Tips",
                                              "Mid Body F/R Triangles",
                                              "Interior Cutoff + Driver Pos",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Right Body High",
                                              "Left Body High",
                                              "Right Wing Mirror",
                                              "Left Wing Mirror",
                                              "Front Right Light Bucket",
                                              "Front Left Light bBucket",
                                              "Front Right Wheel",
                                              "Front Left Wheel",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Front Right Tire",
                                              "Front Left Tire",
                                              "Unknown Invisible",
                                              "Unknown Invisible",
                                              "Rear Right Wheel",
                                              "Rear Left Wheel"};

class NFS4PS1
{
public:
    static std::shared_ptr<Car> LoadCar(const std::string &carVivPath);
    static std::shared_ptr<NFS3_4_DATA::PS1::TRACK> LoadTrack(const std::string &trackGrpPath);

private:
    static std::vector<CarModel> LoadGEO(const std::string &geoPath, std::map<unsigned int, Texture> carTextures);
    static void LoadGRP(const std::string &grpPath);
};
