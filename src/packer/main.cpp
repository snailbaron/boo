#include "schema_generated.h"

#include <arg.hpp>

#include <yaml-cpp/yaml.h>

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

std::vector<uint8_t> readFile(const std::filesystem::path& path)
{
    auto content = std::vector<std::uint8_t>{};

    auto stream = std::ifstream{path, std::ios::binary | std::ios::ate};
    stream.exceptions(std::ios::badbit | std::ios::failbit);
    content.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char*>(content.data()), content.size());
    stream.close();

    return content;
}

void writeFile(
    std::span<const uint8_t> data, const std::filesystem::path& path)
{
    auto stream = std::ofstream{path, std::ios::binary};
    stream.exceptions(std::ios::badbit | std::ios::failbit);
    stream.write(reinterpret_cast<const char*>(data.data()), data.size());
    stream.close();
}

std::string spriteNameToValueName(std::string spriteName)
{
    spriteName[0] = std::toupper(spriteName.at(0));
    return spriteName;
}

struct FrameName {
    std::string object;
    std::string tag;
    int frame = 0;
};

FrameName parseFrameName(std::string_view name)
{
    auto sep1 = name.find(':');
    auto sep2 = name.find(':', sep1 + 1);

    auto frameString = name.substr(sep2 + 1);
    auto frameIndex = std::stoi(std::string{frameString});

    return FrameName{
        .object = std::string{name.substr(0, sep1)},
        .tag = std::string{name.substr(sep1 + 1, sep2 - sep1 - 1)},
        .frame = frameIndex,
    };
}

struct Paths {
    std::filesystem::path source;
    std::filesystem::path data;
    std::filesystem::path header;
};

void pack(const Paths& paths)
{
    auto builder = flatbuffers::FlatBufferBuilder{};

    auto spritesheetData = readFile(paths.source / "images" / "sheet.png");
    auto fbSpritesheet = builder.CreateVector(spritesheetData);

    auto sheetYaml =
        YAML::LoadFile((paths.source / "images" / "sheet.json").string());


    std::map<std::string, std::vector<fb::Frame>> spriteFrames;
    for (const auto& frameYaml : sheetYaml["frames"]) {
        auto [object, tag, frameIndex] = parseFrameName(
            frameYaml["filename"].as<std::string>());

        auto fbFrame = fb::Frame{
            frameYaml["frame"]["x"].as<int32_t>(),
            frameYaml["frame"]["y"].as<int32_t>(),
            frameYaml["frame"]["w"].as<int32_t>(),
            frameYaml["frame"]["h"].as<int32_t>(),
            frameYaml["duration"].as<int32_t>()
        };

        auto& frames = spriteFrames[object];
        if (frameIndex >= frames.size()) {
            frames.resize(frameIndex + 1);
        }
        frames[frameIndex] = fbFrame;

    }

    auto spriteNames = std::vector<std::string>{};
    auto sprites = std::vector<flatbuffers::Offset<fb::Sprite>>{};
    for (const auto& [name, frames] : spriteFrames) {
        auto fbName = builder.CreateString(name);
        auto fbFrames = builder.CreateVectorOfStructs(frames);
        sprites.push_back(fb::CreateSprite(builder, fbName, fbFrames));
        spriteNames.push_back(name);
    }

    auto fbSprites = builder.CreateVector(sprites);

    auto resources = fb::CreateResources(
        builder,
        fbSpritesheet,
        fbSprites
    );

    builder.Finish(resources);

    writeFile(builder.GetBufferSpan(), paths.data);

    auto headerFile = std::ofstream{paths.header};
    headerFile <<
        "#pragma once\n"
        "\n"
        "namespace r {\n"
        "\n"
        "enum class Sprite {\n";

    for (const auto& name : spriteNames) {
        headerFile << "    " << spriteNameToValueName(name) << ",\n";
    }

    headerFile <<
        "};\n"
        "\n"
        "} // namespace r\n";

    headerFile.close();
}

int main(int argc, char* argv[]) try
{
    auto source = arg::option<std::filesystem::path>()
        .keys("--source")
        .markRequired()
        .metavar("DIR")
        .help("path to source directory with assets");
    auto outputDataFilePath = arg::option<std::filesystem::path>()
        .keys("--data")
        .markRequired()
        .metavar("FILE")
        .help("path to output data file");
    auto outputHeaderFilePath = arg::option<std::filesystem::path>()
        .keys("--header")
        .markRequired()
        .metavar("FILE")
        .help("path to output header with resource ids");
    arg::helpKeys("-h", "--help");
    arg::parse(argc, argv);

    pack(Paths{
        .source = source,
        .data = outputDataFilePath,
        .header = outputHeaderFilePath,
    });
} catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
}