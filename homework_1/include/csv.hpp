#ifndef CSV_H_
#define CSV_H_

#include <sstream>
#include <string>
#include <vector>

class CSV {
 public:
  CSV() noexcept {};
  CSV(const CSV&) noexcept = default;
  CSV(CSV&&) noexcept = default;
  CSV& operator=(const CSV&) noexcept = default;
  CSV& operator=(CSV&&) noexcept = default;

  bool Write(const std::string& path) const;
  void Escape(std::string& str) const;
  CSV& Add(std::string str);
  CSV& NewRow();

  void ReadNextLine(std::istream& stream);
  std::istream& Parse(std::istream& stream);

  // returns the string at index position (counting from zero)
  std::string operator[](std::size_t index) const {
    int shift = (index == 0 ? 0 : 1);
    return line_.substr(positions_[index] + shift, positions_[index + 1] - positions_[index] - shift);
  }

  // returns amount of elements in a row
  // note that zero and last indices are in positions_
  size_t Amount() const noexcept { return positions_.size() - 1; }

  void Reset() noexcept {
    row_is_empty_ = true;
    row_is_first_ = true;
    ss_.str(std::string());
    line_.clear();
    positions_.clear();
  }

 private:
  static constexpr char delimeter_ = ',';

  std::stringstream ss_;
  bool row_is_empty_ = true;
  bool row_is_first_ = true;

  std::string line_;
  std::vector<int> positions_;
};

#endif  // CSV_H_
