#pragma once
#ifndef _PATTERN_H
#define _PATTERN_H

#ifdef __cplusplus

enum Duration {
  D4, // quarter
  D8, // eigth
  D16 // sixteenth
};

enum ElementType { Note, Pause };

struct Element {
  ElementType type;
  Duration duration;
};

class Pattern
{
public:
  Pattern() {}
  ~Pattern() {}

  inline std::vector<Element> GetPattern() { return m_pattern; }
  int GetPatternLen();
  void Add(Element e) { m_pattern.push_back(e); }
  void Clear() { m_pattern.clear(); }
  void DeleteLast();
  size_t Size() { return m_pattern.size(); }
  Element Get(size_t i) { return m_pattern[i]; }
  int GetElementLen(Element el);

  std::string &Generate(bool save = true);

private:
  std::vector<Element> m_pattern;
  std::string m_buffer;
};

#endif
#endif