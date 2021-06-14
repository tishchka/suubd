#include <csv.hpp>
#include <fstream>

bool CSV::Write(const std::string& path) const {
  std::fstream output(path, std::ios::out | std::ios::trunc);
  if (!output.is_open()) {
    throw std::runtime_error("CSV::Write: could not open the file " + path);
  }

  output << ss_.str();
  output.close();

  return output.good();
}

void CSV::Escape(std::string& str) const {
  size_t position = str.find("\"");
  bool found_quotes = position != std::string::npos;

  while (position != std::string::npos) {
    str.insert(position, "\"");
    position = str.find("\"", position + 2);
  }

  if (found_quotes || str.find(delimeter_) != std::string::npos) {
    str = "\"" + str + "\"";
  }
}

CSV& CSV::Add(std::string str) {
  if (str.empty()) {
    throw std::invalid_argument("CSV::Add: given empty string");
  }

  Escape(str);

  if (!row_is_empty_) ss_ << delimeter_;
  ss_ << str;
  row_is_empty_ = false;

  return *this;
}

CSV& CSV::NewRow() {
  if (!(row_is_empty_ && row_is_first_)) ss_ << std::endl;
  row_is_first_ = false;
  row_is_empty_ = true;

  return *this;
}

void CSV::ReadNextLine(std::istream& stream) {
  std::getline(stream, line_);

  positions_.clear();
  positions_.push_back(0);
  size_t pos = 0;
  while ((pos = line_.find(delimeter_, pos)) != std::string::npos) {
    positions_.push_back(pos);
    ++pos;
  }

  positions_.push_back(line_.size());
}

std::istream& CSV::Parse(std::istream& stream) {
    ReadNextLine(stream);
    return stream;
  }
