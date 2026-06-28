#include "text_provider.hpp"
#include <fstream>
#include <random>

void replace_all(std::string &str, const std::string &from,
                 const std::string &to) {
  if (from.empty())
    return;

  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

void sanatize_word(std::string &word) {
  replace_all(word, "’", "'");   // Right single quote / Apostrophe
  replace_all(word, "‘", "'");   // Left single quote
  replace_all(word, "”", "\"");  // Right double quote
  replace_all(word, "“", "\"");  // Left double quote
  replace_all(word, "—", "-");   // Em dash
  replace_all(word, "…", "..."); // Ellipsis
}

bool is_sentence_end(const std::string &word) {
  if (word.empty())
    return false;

  char last = word.back();

  if ((last == '"' || last == '\'') && word.size() > 1) {
    last = word[word.size() - 2];
  }

  return last = '.' || last == '?' || last == '!';
}

SystemDictonaryProvider::SystemDictonaryProvider(const std::string &filepath) {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    words.push_back("Error:");
    words.push_back("Dictionary");
    words.push_back("not");
    words.push_back("found.");
    return;
  }

  std::string word;
  while (file >> word) {
    if (word.length() <= 8 && word.find('\'') == std::string::npos) {
      words.push_back(word);
    }
  }
}

SystemDictonaryProvider::~SystemDictonaryProvider() {}

std::string SystemDictonaryProvider::get_next_word() {
  if (words.empty())
    return "Empty ";

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, words.size() - 1);

  return words[distrib(gen)] + " ";
}

bool SystemDictonaryProvider::has_next() const { return !words.empty(); }

CustomBookProvider::CustomBookProvider(const std::string &filepath_in)
    : filepath(filepath_in), current_position(0) {

  std::ifstream file(filepath);

  if (!file.is_open()) {
    words.push_back("Error:");
    words.push_back("Could");
    words.push_back("not");
    words.push_back("open");
    words.push_back("book.");
    return;
  } else {
    std::string word;
    while (file >> word) {
      sanatize_word(word);
      words.push_back(word);
    }
  }

  generate_new_snippet();
}

void CustomBookProvider::generate_new_snippet() {
  if (words.empty())
    return;

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, words.size() - 1);

  size_t start_idx = distrib(gen);

  size_t back_idx = start_idx;
  size_t dist_back = 0;
  while (back_idx > 0 && !is_sentence_end(words[back_idx])) {
    back_idx--;
    dist_back++;
  }

  size_t fwd_idx = start_idx;
  size_t dist_fwd = 0;
  while (fwd_idx < words.size() - 1 && !is_sentence_end(words[fwd_idx])) {
    fwd_idx++;
    dist_fwd++;
  }

  size_t actual_start;
  if (dist_back <= dist_fwd)
    actual_start = (back_idx + 1 < words.size()) ? back_idx + 1 : 0;
  else
    actual_start = (fwd_idx + 1 < words.size()) ? fwd_idx + 1 : 0;

  std::uniform_int_distribution<int> sentence_distrib(3, 4);
  int target_sentences = sentence_distrib(gen);
  int sentence_count = 0;
  size_t current_idx = actual_start;

  while (current_idx < words.size() && sentence_count < target_sentences) {
    current_snippet.push(words[current_idx]);

    if (is_sentence_end(words[current_idx])) {
      sentence_count++;
    }
    current_idx++;
  }
}

CustomBookProvider::~CustomBookProvider() {}

std::string CustomBookProvider::get_next_word() {
  if (current_snippet.empty()) {
    generate_new_snippet();
  }

  if (current_snippet.empty()) {
    return "Empty ";
  }

  std::string next_word = current_snippet.front();
  current_snippet.pop();

  return next_word + " ";
}

bool CustomBookProvider::has_next() const { return !words.empty(); }
