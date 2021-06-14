#ifndef DB_HPP_
#define DB_HPP_

#include <basic.hpp>
#include <csv.hpp>
#include <individual.hpp>
#include <iostream>
#include <student.hpp>
#include <unordered_set>

class DB {
 public:
  DB() noexcept {}

  void Init(const std::string& dir);

  void Save(const std::string& id = std::string());
  void Load(const std::string& id);
  void Create(const std::string& id);

  void AppendBasic() noexcept;
  void AppendIndividual() noexcept;

  void ListDB() const noexcept;
  void ListDBs() const noexcept;
  void PrintRecord(const Student& s) const noexcept;

  void Status() const noexcept;

  void SortByName() noexcept;
  void SortBySemester() noexcept;

  size_t BasicPeek() noexcept;
  size_t IndividualPeek() noexcept;

  Student& BasicGet(size_t index) noexcept;
  Student& IndividualGet(size_t index) noexcept;

  void FocusOnSelection(bool selection_is_basic) noexcept {
    on_selection_ = true;
    selection_is_basic_ = selection_is_basic;
  }

  void GroupByDepartment(const std::string& dep) const;
  void GroupByDiscipline(const std::string& dis) const;

  void RemoveBasic(size_t index) noexcept;
  void RemoveIndividual(size_t index) noexcept;

  void RemoveDB(const std::string& id) noexcept;

  void RenameDB(const std::string& from, const std::string& to) noexcept;

  bool Unset() const noexcept {
    return (id_.empty() && on_selection_);
  }

 private:
  void reset_() {
    id_.clear();
    selection_.clear();
    on_selection_ = false;
    selection_is_basic_ = true;
  }

  // read via the CSV class data from the stream and return scanned Students
  std::vector<Student> read_(std::istream& stream) const;
  void write_(CSV& csv, const std::vector<Student>& students) const;

  std::string dir_;
  std::string id_;

  inline static BasicPlan basic_;
  inline static IndividualPlan individual_;

  bool on_selection_ = false;
  bool selection_is_basic_ = true;
  inline static std::vector<Student> selection_;

  std::unordered_set<std::string> db_set_;
  static constexpr const char* format_ = ".txt";
  static constexpr const char* b_subdir_ = "/basic";
  static constexpr const char* i_subdir_ = "/individual";
};

#endif  // DB_HPP_
