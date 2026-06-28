#pragma once

#include <queue>
#include <string>
#include <vector>

// Abstract class that provides interfaces for both the custom book
// and the word dictionary
class TextProvider {
public:
  virtual ~TextProvider() = default;
  virtual std::string get_next_word() = 0;
  virtual bool has_next() const = 0;
};

// System dict
class SystemDictonaryProvider : public TextProvider {
private:
  std::vector<std::string> words;

public:
  SystemDictonaryProvider(
      const std::string &filepath = "/usr/share/dict/words");
  ~SystemDictonaryProvider() override;

  std::string get_next_word() override;
  bool has_next() const override;
};

// Custom books
class CustomBookProvider : public TextProvider {

private:
  std::string filepath;
  size_t current_position;
  std::vector<std::string> words;
  std::queue<std::string> current_snippet;

  void generate_new_snippet();

public:
  CustomBookProvider(const std::string &filepath);
  ~CustomBookProvider() override;

  std::string get_next_word() override;
  bool has_next() const override;
};
