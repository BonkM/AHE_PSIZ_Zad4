#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

class logging {
  public:
    logging(): f(std::ofstream("log.txt", std::ios_base::out | std::ios_base::binary | std::ios_base::app)) {
      if (!f.good()) {
        throw std::runtime_error("opening log file failed");
      }
    }
    logging(const logging&) = delete;
    logging& operator =(const logging&) = delete;

  template <typename ... Ts>
    void print(Ts&&... vals) {
      auto now = std::chrono::system_clock::now();
      this->f << std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() << " ";
      ((this->f << std::forward<Ts>(vals)), ...);
      this->f << '\n' << std::flush;
    }
  private:
    std::ofstream f;
};

char read_char(std::istream& f) {
  if (!f.good()) {
    if (!f.eof()) {
      throw std::runtime_error("reading error");
    }
    return 0;
  }
  char c = 0;
  f.get(c);
  if (!f.good()) {
    if (!f.eof()) {
      throw std::runtime_error("reading error");
    }
    return 0;
  }
  return c;
}

uint8_t errors_in_byte(std::istream& f1, std::istream& f2) {
  char c1 = read_char(f1);
  char c2 = read_char(f2);
  bool f1_eof = f1.eof(), f2_eof = f2.eof();
  if (f1_eof && f2_eof) {
    return 0;
  }
  if (f1_eof || f2_eof) {
    return 8;
  }
  uint8_t d = ((uint8_t)c1) ^ ((uint8_t)c2);
  uint8_t errs = ((d>>0)&1) + ((d>>1)&1) + ((d>>2)&1) + ((d>>3)&1) + ((d>>4)&1) + ((d>>5)&1) + ((d>>6)&1) + ((d>>7)&1);
  return errs;
}

int main(int argc, char** argv) {
  std::unique_ptr<logging> log;
  try {
    log = std::make_unique<logging>();
  } catch (const std::exception& e) {
    std::cerr << "Error: logging initialization failed: " << e.what() << std::endl;
    return 2;
  } catch (...) {
    std::cerr << "Error: logging initialization failed" << std::endl;
    return 2;
  }

  try {
    if (argc < 3) {
      std::cerr << "Usage: ber <file 1> <file 2>" << std::endl;
      return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    log->print("opening file1: ", argv[1]);
    std::ifstream f1(argv[1]);
    log->print("opening file2: ", argv[2]);
    std::ifstream f2(argv[2]);

    log->print("starting BER analysis");
    uint64_t total = 0;
    uint64_t errs = 0;
    while(!f1.eof() || !f2.eof()) {
      errs += errors_in_byte(f1, f2);
      total += 8;
    }
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    log->print("BER analysis completed",
      ", total_bits:", total,
      ", different_bits:", errs,
      ", ber:", errs / elapsed,
      ", time:", elapsed
    );
    std::cout << "Total bits: " << total << " b" << '\n';
    std::cout << "Different bits: " << errs << " b" << '\n';
    std::cout << "BER: " << errs / elapsed << " b/s" << '\n';
    std::cout << "Time: " << elapsed << " s" << '\n';
    return 0;
  } catch (const std::exception& e) {
    log->print("FATAL ERROR: exception occurred: ", e.what());
    std::cerr << "Error: exception: " << e.what() << std::endl;
    return 2;
  } catch (...) {
    log->print("FATAL ERROR: unknown exception occurred");
    std::cerr << "Error: unknown exception" << std::endl;
    return 2;
  }
}
