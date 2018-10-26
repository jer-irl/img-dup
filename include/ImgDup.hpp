#pragma once
#include "VPTree.hpp"
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <cstdint>
#include <vector>

namespace imgdup {

template<typename HashType>
class ImageProcessor {
public:
    ImageProcessor() = default;

    void processDirectory(boost::filesystem::path path);
    std::vector<boost::filesystem::path> closestForImage(boost::filesystem::path path, std::size_t numClosest);

private:
    struct ImageRecord {
        static double distance(const ImageRecord &a, const ImageRecord &b) {
            auto func = HashType::create();
            return func->compare(a.hash, b.hash);
        }

        ImageRecord(boost::filesystem::path p, cv::Mat h) : path(p), hash(h) {}

        boost::filesystem::path path;
        cv::Mat hash;
    };

    VpTree<ImageRecord, &ImageRecord::distance> tree_;
};

template<typename HashType>
void ImageProcessor<HashType>::processDirectory(boost::filesystem::path path) {
    namespace fs = boost::filesystem;

    std::vector<ImageRecord> records;
    for (fs::directory_entry &entry : fs::recursive_directory_iterator(path)) {
        if (!fs::is_regular_file(entry.path())) {
            continue;
        } else if (entry.path().filename().string()[0] == '.') {
            continue;
        }

        const std::vector<std::string> acceptableExtensions{".jpg", ".jpeg", ".png"};
        bool isAcceptableExtension = std::any_of(
                acceptableExtensions.cbegin(), acceptableExtensions.cend(), [&entry](const std::string &ext){
                    return ext == entry.path().extension();
                });

        if (!isAcceptableExtension) {
            continue;
        }

        cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_COLOR);
        cv::Mat hash;
        auto func = HashType::create();
        func->compute(img, hash);

        records.emplace_back(entry.path(), hash);
    }
    tree_.create(records);
}

template<typename HashType>
std::vector<boost::filesystem::path> ImageProcessor<HashType>::closestForImage(boost::filesystem::path path, std::size_t numClosest) {
    namespace fs = boost::filesystem;

    std::vector<ImageRecord> results;
    std::vector<double> distances;

    cv::Mat img = cv::imread(path.string(), cv::IMREAD_COLOR);
    cv::Mat hash;
    auto func = HashType::create();
    func->compute(img, hash);

    tree_.search(ImageRecord{path, hash}, numClosest, &results, &distances);

    std::vector<fs::path> res;
    std::transform(results.cbegin(), results.cend(), std::back_inserter(res), [](const ImageRecord &record){return record.path;});
    return res;
}

} // ns imgdup