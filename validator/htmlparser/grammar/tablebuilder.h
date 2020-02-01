//
// Copyright 2019 The AMP HTML Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the license.
//

#ifndef HTMLPARSER__GRAMMAR_TABLEBUILDER_H_
#define HTMLPARSER__GRAMMAR_TABLEBUILDER_H_

//
// NOTE: This is very basic right now. No validation of grammer syntax. Lack of
// verbose errors and not well tested. Parsers using table generated by this
// class must be thoroughly tested.
//
// Builds a state table by reading grammar file that contains rules for parsing
// a basic (limited), context free, unambigous grammar.
//
// Using TableBuilder one can generate parser states by writing rules in a
// text file. See htmlparser/data/jsongrammar.txt.
//
// Grammar text file contains rules which lists states and its transition
// from one state to another as parser reads input characters. The parse
// table is pushdown automation that uses stack to push and pop parsing
// states. Unline LR parsers there is no shift at each stage of parsing.

// See grammar.txt tutorial for learning grammar syntax.
// TODO(amaltas): Add grammer tutorial.
//
// Notes:
//  - Supports up to 255 unique states.
//  - Supports up to 255 unique callbacks.
//  - Support for only ascii chars grammar.

#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace htmlparser::grammar {

struct Rule {
  std::string state;
  std::vector<char> input;
  std::vector<std::string> transition;
};

class TableBuilder  {
 public:
  struct OutputFileOptions {
    std::string output_file_path;
    std::string license_header;
    std::string ifdef_guard;
    std::string cpp_namespace;
  };

  TableBuilder(std::string_view grammar_file_path, OutputFileOptions options);

  bool ParseRulesAndGenerateTable();

 private:
  bool OutputHeaderFile(const std::set<std::string>& declared_states,
                        const std::array<int, 127>& tokenindexes,
                        const std::map<uint8_t, std::vector<uint32_t>>& table);

  // The state code contains information about the state, transition,
  // shifting and callbacks.
  //
  // lowest 6 bits are reserved for future use.
  // 7th bit contains pop bit.
  // 8th bit contains push bit.
  // next 8 bits contains active state code.
  // next 8 bits contains shift state code.
  // next 8 bits contains callback code.
  //
  // 0b11111111  11111111   11111111   11           111111
  // ----------  --------   --------   --           ------
  //     |           |         |       |              |
  // Callback  shift code  state  Push|Pop bits  Reserved.
  //
  std::optional<uint32_t> ComputeState(uint8_t row, Rule r);
  bool ParseGrammarFile();
  void RemoveLeadingWhitespace(std::string_view* line) const;
  std::optional<Rule> ReadRule(std::string_view line, int line_no) const;
  std::optional<uint32_t> ComputeStateBits(uint8_t callback_code,
                                           uint8_t push_state_code,
                                           uint8_t current_state_code,
                                           bool push,
                                           bool pop);

  std::vector<Rule> raw_rules_;
  std::string grammar_file_path_;
  OutputFileOptions header_options_;
  std::map<std::string, uint8_t> state_codes_;
  std::set<char> charset_;
  uint8_t state_code_counter_ = 0;
};

}  // namespace htmlparser::grammar


#endif  // HTMLPARSER__GRAMMAR_TABLEBUILDER_H_
