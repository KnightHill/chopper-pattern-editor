#include <cstdlib>
#include <string>
#include <vector>
#include <format>
#include <fstream>
#include "pattern.h"

using namespace std;

const char *PATTERN_FILE = "pattern.txt";
const char *README_FILE = "readme.txt";

void Pattern::DeleteLast()
{
  if (m_pattern.size() > 0)
    m_pattern.pop_back();
}

int Pattern::GetElementLen(Element el)
{
  switch (el.duration) {
  case D16:
  default:
    return 1;
  case D8:
    return 2;
  case D4:
    return 4;
  }
}

int Pattern::GetPatternLen()
{
  int len = 0;
  for (size_t i = 0; i < m_pattern.size(); i++) {
    Element el = m_pattern[i];
    len += GetElementLen(el);
  }
  return len;
}

string &Pattern::GenerateCode(bool save)
{
  m_buffer.clear();
  m_buffer.append("{");
  m_buffer.append(format("{},{{", static_cast<int>(m_pattern.size())));

  for (size_t i = 0; i < m_pattern.size(); i++) {
    Element el = m_pattern[i];
    m_buffer.append(format("{{{},", el.type == Note ? 1 : 0));

    switch (el.duration) {
    case D4:
      m_buffer.append("D4}");
      break;
    case D8:
      m_buffer.append("D8}");
      break;
    case D16:
      m_buffer.append("D16}");
      break;
    }
    if (i < m_pattern.size() - 1)
      m_buffer.append(",");
  }

  m_buffer.append("}},");

  if (save) {
    fstream fs;
    fs.open(PATTERN_FILE, std::fstream::out | std::fstream::app);
    fs << m_buffer << endl;
    fs.close();
  }

  return m_buffer;
}

string &Pattern::GenerateReadme(bool save)
{
  m_buffer.clear();

  m_buffer.append("<tr>");
  m_buffer.append("<td>PATTERN NO</td>");
  m_buffer.append("<td>LED1 COLOR</td>");
  m_buffer.append("<td>LED1 COLO2</td>");
  m_buffer.append("</tr>");

  for (size_t i = 0; i < m_pattern.size(); i++) {
    Element el = m_pattern[i];
    switch (el.duration) {
    case D16:
      m_buffer.append("<td>");
      break;
    case D8:
      m_buffer.append("<td colspan=\"2\">");
      break;
    case D4:
      m_buffer.append("<td colspan=\"4\">");
      break;
    }

    m_buffer.append(el.type == Note ? "1" : "0");
    m_buffer.append("</td>");
  }

  if (save) {
    fstream fs;
    fs.open(README_FILE, std::fstream::out | std::fstream::app);
    fs << m_buffer << endl;
    fs.close();
  }

  return m_buffer;
}
