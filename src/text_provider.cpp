#include "text_provider.hpp"

SystemDictonaryProvider::SystemDictonaryProvider(const std::string &filepath) {}
SystemDictonaryProvider::~SystemDictonaryProvider() {}

std::string SystemDictonaryProvider::get_next_word() { return "Hello World"; }
bool SystemDictonaryProvider::has_next() const { return true; }

CustomBookProvider::CustomBookProvider(const std::string &filepath) {}
CustomBookProvider::~CustomBookProvider() {}

std::string CustomBookProvider::get_next_word() { return "Hello World"; }
bool CustomBookProvider::has_next() const { return true; }
