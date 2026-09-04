// Script to convert Meta's block storage traces to libCacheSim csv format
//
// Usage:
// g++ conv_meta_block.cpp -o conv_meta_block
// ./conv_meta_block block_traces_1_conved.csv block_traces_1.csv

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_set>
#include <vector>

constexpr size_t CHUNK_SIZE = 128 * 1024;  // 128 KiB
constexpr double SAMPLE_RATIO = 0.1;  // sample SAMPLE_RATIO fraction of blocks
constexpr size_t SAMPLE_SEED = 42;

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " <output_csv> <input_csv1> [<input_csv2> ...]\n";
    return EXIT_FAILURE;
  }

  // open files
  std::ofstream output_csv{argv[1]};
  if (!output_csv) {
    std::cout << "Error: Could not open file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  std::vector<std::ifstream> input_csvs{};
  for (size_t i = 2; i < argc; ++i) {
    input_csvs.emplace_back(argv[i]);
    if (!input_csvs.back()) {
      std::cout << "Error: Could not open file " << argv[i] << std::endl;
    }
  }

  // vars for sampling
  std::unordered_set<std::string> keep{};
  std::unordered_set<std::string> dont_keep{};
  std::mt19937 gen{SAMPLE_SEED};
  std::uniform_real_distribution<> dis(0.0, 1.0);

  // header of output csv, following libCacheSim csv format
  output_csv << "timestamp,obj_id,obj_size\n";

  // fields of input csvs we are interested in, following Meta's block storage
  // traces format
  size_t op_time;           // 0
  std::string block_id;     // 1
  size_t io_size;           // 3
  size_t io_offset;         // 4
  std::string rs_shard_id;  // 10

  // read input_csvs, write to output_csv
  std::string line;
  std::string cell;
  std::vector<std::string> cells{};
  for (auto& input_csv : input_csvs) {
    getline(input_csv, line);  // skip header
    while (getline(input_csv, line)) {
      cells.clear();

      std::stringstream row(line);
      while (getline(row, cell, ',')) {
        cells.push_back(cell);
      }

      op_time = std::stoull(cells[0]);
      block_id = cells[1];
      io_size = std::stoull(cells[3]);
      io_offset = std::stoull(cells[4]);
      rs_shard_id = cells[10];

      // Reference for doing conversion:
      // https://github.com/facebook/CacheLib/commit/23a888e54e4fed22f81c114e3ca9af95d7f0787c
      // Each storage node block is uniquely identified via block_id +
      // rs_shard_id. It's in turn broken into chunks to be cached. We
      // represent a chunk of a storage node block as the cache object.
      std::string real_block_id = block_id + "_" + rs_shard_id;
      if (keep.count(real_block_id) == 0 &&
          dont_keep.count(real_block_id) == 0) {
        double u = dis(gen);
        if (u > SAMPLE_RATIO) {
          dont_keep.insert(real_block_id);
        } else {
          keep.insert(real_block_id);
        }
      }

      if (dont_keep.count(real_block_id) > 0) {
        continue;
      }

      size_t start_chunk = io_offset / CHUNK_SIZE;
      size_t end_chunk = (io_offset + io_size - 1) / CHUNK_SIZE;
      for (size_t chunk_id = start_chunk; chunk_id <= end_chunk; ++chunk_id) {
        std::string obj_id = real_block_id + "_" + std::to_string(chunk_id);
        output_csv << op_time << ',' << obj_id << ',' << CHUNK_SIZE << '\n';
      }
    }
  }
}
