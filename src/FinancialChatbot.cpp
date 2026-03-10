#include "FinancialChatbot.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace {

// Escape a string for use inside a JSON "..." value.
std::string jsonEscape(const std::string &s) {
  std::string out;
  out.reserve(s.size() + 8);
  for (char c : s) {
    switch (c) {
    case '\\':
      out += "\\\\";
      break;
    case '"':
      out += "\\\"";
      break;
    case '\n':
      out += "\\n";
      break;
    case '\r':
      out += "\\r";
      break;
    case '\t':
      out += "\\t";
      break;
    default:
      out += c;
      break;
    }
  }
  return out;
}

// Read GEMINI_API_KEY from .env in the current directory (KEY=value).
std::string readGeminiKeyFromEnvFile() {
  std::ifstream f(".env");
  if (!f)
    return "";
  std::string line;
  while (std::getline(f, line)) {
    size_t start = line.find_first_not_of(" \t\r\n");
    if (start == std::string::npos || line[start] == '#')
      continue;
    size_t eq = line.find('=', start);
    if (eq == std::string::npos)
      continue;
    std::string key = line.substr(start, eq - start);
    // Trim key
    size_t keyEnd = key.find_last_not_of(" \t");
    if (keyEnd != std::string::npos)
      key = key.substr(0, keyEnd + 1);

    if (key == "GEMINI_API_KEY") {
      std::string value = line.substr(eq + 1);
      size_t vStart = value.find_first_not_of(" \t\"");
      if (vStart == std::string::npos)
        continue;
      size_t vEnd = value.find_last_not_of(" \t\"\r\n");
      if (vEnd == std::string::npos || vEnd < vStart)
        continue;
      return value.substr(vStart, vEnd - vStart + 1);
    }
  }
  return "";
}

} // namespace

void FinancialChatbot::setContext(const SmartAnalyzer::AnalysisResult &result,
                                  double balance, const Goal &goal) {
  m_result = result;
  m_balance = balance;
  m_goal = goal;
}

std::string FinancialChatbot::categoryToString(SpendingCategory category) {
  switch (category) {
  case SpendingCategory::Income:
    return "Income";
  case SpendingCategory::Housing:
    return "Housing";
  case SpendingCategory::Utilities:
    return "Utilities";
  case SpendingCategory::Groceries:
    return "Groceries";
  case SpendingCategory::Dining:
    return "Dining";
  case SpendingCategory::Transport:
    return "Transport";
  case SpendingCategory::Entertainment:
    return "Entertainment";
  case SpendingCategory::Healthcare:
    return "Healthcare";
  case SpendingCategory::Education:
    return "Education";
  case SpendingCategory::Savings:
    return "Savings";
  case SpendingCategory::Other:
    return "Other";
  }
  return "Unknown";
}

std::string FinancialChatbot::buildSystemContext() const {
  std::ostringstream os;
  os << std::fixed << std::setprecision(2);
  os << "You are a helpful financial insights assistant for a bank customer. ";
  os << "Use ONLY the following data about the customer. Do not invent "
        "numbers.\n\n";
  os << "Current balance: " << m_balance << "\n";
  os << "Financial health score (0-100): " << std::setprecision(1)
     << m_result.financialHealthScore << "\n";
  os << "Savings ratio (% of income): " << m_result.savingsRatio << "\n";
  os << "Overspending (discretionary >35%): "
     << (m_result.isOverspending ? "yes" : "no") << "\n";
  os << "Budget usage >80%: " << (m_result.isBudgetOver80 ? "yes" : "no")
     << "\n";
  os << "High risk (savings <15%): " << (m_result.isHighRisk ? "yes" : "no")
     << "\n\n";

  if (!m_result.categoryExpenseShare.empty()) {
    os << "Expense share by category (%):\n";
    for (const auto &e : m_result.categoryExpenseShare)
      os << " - " << categoryToString(e.first) << ": " << e.second << "%\n";
    os << "\n";
  }
  if (!m_result.warnings.empty()) {
    os << "Warnings:\n";
    for (const auto &w : m_result.warnings)
      os << " - " << w << "\n";
    os << "\n";
  }
  if (!m_result.recommendations.empty()) {
    os << "Recommendations:\n";
    for (const auto &r : m_result.recommendations)
      os << " - " << r << "\n";
    os << "\n";
  }
  if (m_goal.getTargetAmount() > 0.0) {
    os << "Savings goal: \"" << m_goal.getName() << "\", target $"
       << m_goal.getTargetAmount() << " in " << m_goal.getTargetMonths()
       << " months, progress " << std::setprecision(1)
       << (m_goal.getProgressRatio() * 100.0) << "%.\n";
  }
  os << "\nAnswer the user's question based on this data. Be concise and "
        "helpful.";
  return os.str();
}

std::string FinancialChatbot::respond(const std::string &userMessage) const {
  std::string apiKey;
  const char *keyEnv = std::getenv("GEMINI_API_KEY");
  if (keyEnv && keyEnv[0])
    apiKey = keyEnv;
  else
    apiKey = readGeminiKeyFromEnvFile();

  if (apiKey.empty()) {
    return "GEMINI_API_KEY not found. Set it in .env or as an environment "
           "variable.";
  }

  std::string systemContent = buildSystemContext();

  {
    std::ofstream ofs("ai_input.json");
    if (ofs) {
      ofs << "{"
          << "\"api_key\":\"" << jsonEscape(apiKey) << "\","
          << "\"system_message\":\"" << jsonEscape(systemContent) << "\","
          << "\"user_message\":\"" << jsonEscape(userMessage) << "\""
          << "}";
    }
  }

  std::string command = "python ai_bridge.py ai_input.json 2>&1";
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe)
    return "Error: Could not execute Python AI bridge.";

  std::string output;
  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    output += buffer;

  int result = pclose(pipe);
  if (result != 0) {
    if (output.empty())
      return "Error: Python bridge failed with exit code " +
             std::to_string(result);
    return "Error: " + output;
  }

  return output;
}
