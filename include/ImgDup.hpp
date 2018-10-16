#pragma once
#include "VPTree.hpp"
#include <boost/filesystem.hpp>
#include <cstdint>
#include <vector>

namespace imgdup {

class ImageProcessor {
public:
    ImageProcessor() = default;

    void processDirectory(boost::filesystem::path path);
    std::vector<boost::filesystem::path> closestForImage(boost::filesystem::path path, std::size_t numClosest);

private:
    struct ImageRecord {
        static double distance(const ImageRecord &a, const ImageRecord &b) {
#if __has_builtin(__builtin_popcount)
            return __builtin_popcount(a.hash ^ b.hash);
#else
            double result = 0;
            uint64_t tmp = a.hash ^ b.hash;
            while (tmp) {
                if (tmp & 1) {
                    ++result;
                }
                tmp = tmp >> 1;
            }
            return result;
#endif
        }

        ImageRecord(boost::filesystem::path p, uint64_t h) : path(p), hash(h) {}

        boost::filesystem::path path;
        uint64_t hash;
    };

    VpTree<ImageRecord, &ImageRecord::distance> tree_;
};

} // ns imgdup