#ifndef CLI_HPP_
#define CLI_HPP_

#include <algorithm>
#include <db.hpp>
#include <exception>
#include <extra.hpp>
#include <iomanip>
#include <iostream>
#include <prompts.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

std::string DIR_PATH;

// cli_pair: <name of a function, description of a function>
using cli_pair = std::pair<std::string, std::string>;
const std::vector<cli_pair> helper = {
    {"help", "lists available functions"},
    {"exit", "carefully terminates the programm"},
    {"create <id>", "creates database with the provided name"},
    {"save", "saves current database, if one is constructed"},
    {"status", "prints information about the current database"},
    {"ld", "stands for 'list databases'"},
    {"add <b, i>", "add a student with the basic or individual plan"},
    {"load <id>", "loads the <name> database, if such exists"},
    {"ls", "prints the records of the loaded database, if one is loaded"},
    {"rm <id>", "removes the <name> database, if one exists"},
    {"rename <old> <new>", "renames the <old> database, if one exists, to <new> database"},
    {"sort {name, semester}", "sorts students by name or semester"},
    {"edit", "choose a student from the list to edit"},
    {"rm", "choose a student from the list to remove"},
    {"select <dep, dis> <name>", "get <dep> department students or ones who have studied a <dis> discipline"},
    {"sselect <b, i> <id>", "saves current selection, if one exists"},
};

using args = const std::vector<std::string> &;

class Handler {
 public:
  virtual void call(args) = 0;
  virtual ~Handler() noexcept {};
};

class Wrapper {
 public:
  Wrapper() noexcept { db.Init(DIR_PATH); };

  class Help;
  class Exit;
  class Create;
  class Save;
  class Status;
  class ListDBs;
  class Add;
  class Load;
  class ListDB;
  class RemoveDB;
  class RenameDB;
  class Sort;
  class Edit;
  class Remove;
  class Select;
  class SaveSelection;

 private:
  inline static DB db;
};

class Wrapper::Help : public Handler {
 public:
  void call(args) override {
    for (auto x : helper) {
      std::cout << "-- " << std::left << std::setw(50) << Bold(Blue(x.first)) << x.second << std::endl;
    }
  }
};

class Wrapper::Exit : public Handler {
 public:
  // will be caught in main()
  void call(args) override { throw std::exception(); }
};

class Wrapper::Create : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 2) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else {
      db.Create(x[1]);
    }
  }
};

class Wrapper::Save : public Handler {
 public:
  void call(args) override { db.Save(); }
};

class Wrapper::Status : public Handler {
 public:
  void call(args) override { db.Status(); }
};

class Wrapper::ListDBs : public Handler {
 public:
  void call(args) override { db.ListDBs(); }
};

class Wrapper::Add : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 2) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else if (x[1] == "b") {
      db.AppendBasic();
    } else if (x[1] == "i") {
      db.AppendIndividual();
    } else {
      std::cout << Red("error: invalid usage") << std::endl;
    }
  }
};

class Wrapper::Load : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 2) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else {
      db.Load(x[1]);
    }
  }
};

class Wrapper::ListDB : public Handler {
 public:
  void call(args) override { db.ListDB(); }
};

class Wrapper::RemoveDB : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 2) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else {
      db.RemoveDB(x[1]);
    }
  }
};

class Wrapper::RenameDB : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 3) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else {
      db.RenameDB(x[1], x[2]);
    }
  }
};

class Wrapper::Sort : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 2) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else if (x[1] == "name") {
      db.SortByName();
    } else if (x[1] == "semester") {
      db.SortBySemester();
    } else {
      std::cout << Red("error: invalid usage") << std::endl;
    }
  }
};

class Wrapper::Edit : public Handler {
 public:
  void call(args) override {
    if (db.Unset()) {
      std::cout << Red("error: no database or selection is chosen") << std::endl;
      return;
    }

    Student &s = db.BasicGet(db.BasicPeek());
    db.PrintRecord(s);
    s = PromptForStudent();

    s = db.IndividualGet(db.IndividualPeek());
    db.PrintRecord(s);
    s = PromptForStudent();
  }
};

class Wrapper::Remove : public Handler {
 public:
  void call(args) override {
    if (db.Unset()) {
      std::cout << Red("error: no database or selection is chosen") << std::endl;
      return;
    }
    db.RemoveBasic(db.BasicPeek());
    db.RemoveIndividual(db.IndividualPeek());
  }
};

class Wrapper::Select : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 3) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else if (x[1] == "dep") {
      db.GroupByDepartment(x[2]);
    } else if (x[1] == "dis") {
      db.GroupByDiscipline(x[2]);
    } else {
      std::cout << Red("error: invalid usage") << std::endl;
    }
  }
};

class Wrapper::SaveSelection : public Handler {
 public:
  void call(args x) override {
    if (x.size() != 3) {
      std::cout << Red("error: invalid usage") << std::endl;
    } else if (x[1] == "b") {
      db.FocusOnSelection(true);
      db.Save(x[2]);
    } else if (x[1] == "i") {
      db.FocusOnSelection(false);
      db.Save(x[2]);
    } else {
      std::cout << Red("error: invalid usage") << std::endl;
    }
  }
};

class CLI {
 public:
  CLI() noexcept {
    wrapper_ = Wrapper();
    functions_.insert({"help", &help_});
    functions_.insert({"exit", &exit_});
    functions_.insert({"create", &create_});
    functions_.insert({"save", &save_});
    functions_.insert({"status", &status_});
    functions_.insert({"ld", &ld_});
    functions_.insert({"add", &add_});
    functions_.insert({"load", &load_});
    functions_.insert({"ls", &list_});
    functions_.insert({"rm", &remove_db_});
    functions_.insert({"rename", &rename_db_});
    functions_.insert({"sort", &sort_});
    functions_.insert({"edit", &edit_});
    functions_.insert({"rm", &rm_});
    functions_.insert({"select", &select_});
    functions_.insert({"sselect", &sselect_});
  }

  void Process(args x) const {
    auto it = functions_.find(x[0]);
    if (it != functions_.end()) {
      it->second->call(x);
    } else {
      std::cout << Red("cli: command not found: ") << x[0] << std::endl;
    }
  }

 private:
  static Wrapper wrapper_;

  Wrapper::Help help_;
  Wrapper::Exit exit_;
  Wrapper::Create create_;
  Wrapper::Save save_;
  Wrapper::Status status_;
  Wrapper::ListDBs ld_;
  Wrapper::Add add_;
  Wrapper::Load load_;
  Wrapper::ListDB list_;
  Wrapper::RemoveDB remove_db_;
  Wrapper::RenameDB rename_db_;
  Wrapper::Sort sort_;
  Wrapper::Edit edit_;
  Wrapper::Remove rm_;
  Wrapper::Select select_;
  Wrapper::SaveSelection sselect_;

  std::unordered_map<std::string, Handler *> functions_;
};

// commands
/*
    help +
    create +
    list existing +
    remove db +
    rename +
    load +
    save +
    append +
    edit (student) +
    remove (student) +
    ls +
    sort +
    group (by department or discipline)
    save group
*/

#endif  // CLI_HPP_
