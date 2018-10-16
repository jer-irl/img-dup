#include "ImgDup.hpp"
#include <pHash.h>
#include <set>
#include <algorithm>

namespace fs = boost::filesystem;

namespace imgdup {

void ImageProcessor::processDirectory(boost::filesystem::path path) {
    std::vector<ImageRecord> records;
    for (fs::directory_entry &entry : fs::recursive_directory_iterator(path)) {
        if (!fs::is_regular_file(entry.path())) {
            continue;
        }

        ulong64 hash;
        ph_dct_imagehash(entry.path().c_str(), hash);
        records.emplace_back(entry.path(), hash);
    }
    tree_.create(records);
}

std::vector<fs::path> ImageProcessor::closestForImage(boost::filesystem::path path, std::size_t numClosest) {
    std::vector<ImageRecord> results;
    std::vector<double> distances;

    ulong64 hash;
    ph_dct_imagehash(path.c_str(), hash);
    tree_.search(ImageRecord{path, hash}, numClosest, &results, &distances);

    std::vector<fs::path> res;
    std::transform(results.cbegin(), results.cend(), res.begin(), [](const ImageRecord &record){return record.path;});
    return res;
}

} // ns imgdup