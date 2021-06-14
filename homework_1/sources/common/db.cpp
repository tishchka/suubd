#include <algorithm>
#include <csv.hpp>
#include <db.hpp>
#include <extra.hpp>
#include <filesystem>
#include <fstream>
#include <prompts.hpp>
#include <student.hpp>
#include <system_error>

using dir_iterator = std::filesystem::directory_iterator;

void DB::Init(const std::string& dir) {
  dir_ = dir;
  for (const auto& entry : dir_iterator(dir_ + b_subdir_)) {
    db_set_.insert(entry.path().stem());
  }
  for (const auto& entry : dir_iterator(dir_ + i_subdir_)) {
    db_set_.insert(entry.path().stem());
  }
}

void DB::Save(const std::string& id) {
  if (id_.empty() && (!on_selection_ || id.empty())) {
    std::cout << Red("error: no database or selection is chosen") << std::endl;
    return;
  }

  CSV csv;
  bool ok = true;

  std::string s;
  if (on_selection_) {
    s = id;
  } else {
    s = id_;
  }

  if (on_selection_) {
    if (selection_is_basic_) {
      write_(csv, selection_);
      ok = ok && csv.Write(dir_ + b_subdir_ + '/' + s + format_);
      csv.Reset();
    } else {
      write_(csv, selection_);
      ok = ok && csv.Write(dir_ + i_subdir_ + '/' + s + format_);
    }
  } else {
    write_(csv, basic_.Students());
    ok = ok && csv.Write(dir_ + b_subdir_ + '/' + s + format_);
    csv.Reset();
    write_(csv, individual_.Students());
    ok = ok && csv.Write(dir_ + i_subdir_ + '/' + s + format_);
  }

  if (ok) {
    db_set_.insert(s);
    std::cout << Green("successfully saved " + Bold(s) + " database") << std::endl;
    reset_();
  } else {
    std::cout << Red("error: could not save " + Bold(s) + " database") << std::endl;
  }
}

void DB::Load(const std::string& name) {
  bool found = false;
  for (const auto& entry : dir_iterator(dir_ + b_subdir_)) {
    if (entry.path().stem() == name) {
      found = true;
      std::ifstream file(entry.path());
      if (!file.is_open()) {
        std::cout << Red("error: can not open the " + Bold(entry.path()) + " file");
      }
      basic_ = BasicPlan(read_(file));
    }
  }

  for (const auto& entry : dir_iterator(dir_ + i_subdir_)) {
    if (entry.path().stem() == name) {
      found = true;
      std::ifstream file(entry.path());
      if (!file.is_open()) {
        std::cout << Red("error: can not open the " + Bold(entry.path()) + " file");
      }
      individual_ = IndividualPlan(read_(file));
    }
  }

  if (found) {
    id_ = name;
    std::cout << Green("database " + Bold(name) + " is successfully loaded") << std::endl;
  } else {
    std::cout << Red("error: database " + Bold(name) + " does not exist") << std::endl;
  }
}

void DB::Create(const std::string& name) {
  if (std::find(db_set_.begin(), db_set_.end(), name) == db_set_.end()) {
    id_ = name;
    std::cout << Green("database " + Bold(name) + " is created") << std::endl;
  } else {
    std::cout << Red("error: database " + Bold(name) + " already exists") << std::endl;
  }
}

void DB::AppendBasic() noexcept { basic_.AddStudent(PromptForStudent()); }
void DB::AppendIndividual() noexcept { individual_.AddStudent(PromptForStudent()); }

void DB::ListDB() const noexcept {
  if (id_.empty()) {
    std::cout << Red("error: no database is chosen") << std::endl;
  } else {
    std::cout << Bold("basic plan:") << std::endl;
    basic_.ListStudents();
    std::cout << std::endl << Bold("individual plan:") << std::endl;
    individual_.ListStudents();
  }
}

void DB::ListDBs() const noexcept {
  std::cout << Bold("currently existing databases:") << std::endl;
  for (const auto& db : db_set_) {
    std::cout << ' ' << db << std::endl;
  }
}

void DB::PrintRecord(const Student& s) const noexcept { basic_.ListStudent(s); }

void DB::Status() const noexcept {
  std::cout << Bold("directory: ") + Blue(dir_) << std::endl;
  std::cout << Bold("database: ") + Blue(id_) << std::endl;
  std::cout << Bold("amount of db's: ") + Blue(std::to_string(db_set_.size())) << std::endl;
  if (!id_.empty()) {
    std::cout << Bold("amount of students with " + Blue("basic") + " and " + Green("individual") + " plan: ");
    std::cout << Blue(std::to_string(basic_.AmountOfStudents())) + ", ";
    std::cout << Green(std::to_string(individual_.AmountOfStudents())) << std::endl;
  }
  if (on_selection_) {
    std::cout << Bold("amount of students in selection: ") << selection_.size() << std::endl;
  }
}

void DB::SortByName() noexcept {
  basic_.SortByName();
  individual_.SortByName();
}

void DB::SortBySemester() noexcept {
  basic_.SortBySemester();
  individual_.SortBySemester();
}

size_t DB::BasicPeek() noexcept { return basic_.Peek(); }
size_t DB::IndividualPeek() noexcept { return individual_.Peek(); }

Student& DB::BasicGet(size_t index) noexcept { return basic_.GetStudent(index); }
Student& DB::IndividualGet(size_t index) noexcept { return individual_.GetStudent(index); }

void DB::GroupByDepartment(const std::string& dep) const {
  auto sel = basic_.GroupByDepartment(dep);
  selection_.insert(selection_.end(), sel.begin(), sel.end());

  sel = individual_.GroupByDepartment(dep);
  selection_.insert(selection_.end(), sel.begin(), sel.end());
}

void DB::GroupByDiscipline(const std::string& dis) const {
  auto sel = basic_.GroupByDiscipline(dis);
  selection_.insert(selection_.end(), sel.begin(), sel.end());

  sel = individual_.GroupByDiscipline(dis);
  selection_.insert(selection_.end(), sel.begin(), sel.end());
}

void DB::RemoveBasic(size_t index) noexcept { basic_.RemoveStudent(index); }
void DB::RemoveIndividual(size_t index) noexcept { individual_.RemoveStudent(index); }

void DB::RemoveDB(const std::string& id) noexcept {
  if (id == id_) {
    std::cout << Red("error: can't remove current (unstaged) database - save it first") << std::endl;
    return;
  }

  db_set_.erase(id);
  std::string b = dir_ + b_subdir_ + '/' + id + format_;
  std::string i = dir_ + i_subdir_ + '/' + id + format_;
  if (std::remove(b.c_str()) * std::remove(i.c_str()) == 0) {
    std::cout << Green("successfully deleted " + Bold(id) + " database") << std::endl;
  } else {
    std::cout << Red("error: database " + Bold(id) + " does not exist") << std::endl;
  }
}

void DB::RenameDB(const std::string& from, const std::string& to) noexcept {
  if (db_set_.find(from) == db_set_.end()) {
    std::cout << Red("error: database " + Bold(from) + " does not exist") << std::endl;
  } else if (from == id_) {
    id_ = from;
    std::cout << Green("renamed current " + Bold(from) + " database to " + Bold(to)) << std::endl;
  } else {
    std::error_code ec;
    std::filesystem::rename(dir_ + b_subdir_ + '/' + from + format_, dir_ + b_subdir_ + '/' + to + format_, ec);
    if (!ec) std::cout << Green("renamed basic " + Bold(from) + " database to " + Bold(to)) << std::endl;
    std::filesystem::rename(dir_ + i_subdir_ + '/' + from + format_, dir_ + i_subdir_ + '/' + to + format_, ec);
    if (!ec) std::cout << Green("renamed individual " + Bold(from) + " database to " + Bold(to)) << std::endl;
  }
}

std::vector<Student> DB::read_(std::istream& stream) const {
  CSV csv;
  std::vector<Student> students;
  while (csv.Parse(stream)) {
    std::string name = csv[0];
    std::string department = csv[1];
    int semester = std::stoi(csv[2]);

    Student::Disciplines disciplines;
    for (size_t i = 3; i < csv.Amount(); i += 3) {
      disciplines.push_back({{csv[i], std::stoi(csv[i + 1])}, static_cast<Grade>(std::stoi(csv[i + 2]))});
    }

    students.push_back({name, department, semester, disciplines});
  }
  return students;
}

void DB::write_(CSV& csv, const std::vector<Student>& students) const {
  for (auto& student : students) {
    csv.Add(student.GetName()).Add(student.GetDepartment());
    csv.Add(std::to_string(student.GetSemester()));
    for (auto& d : student.GetDisciplines()) {
      csv.Add(d.first.first);
      csv.Add(std::to_string(d.first.second));
      csv.Add(std::to_string(static_cast<int>(d.second)));
    }
    csv.NewRow();
  }
}
