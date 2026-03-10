#pragma once

#include <string>

#include "Goal.h"
#include "SmartAnalyzer.h"

// Chatbot that sends user messages to the Gemini API (Google Generative AI)
// with the user's financial context; responses come from Gemini. Set
// GEMINI_API_KEY in the environment to enable.
class FinancialChatbot {
public:
  // Set the current context (call after runAnalysis).
  void setContext(const SmartAnalyzer::AnalysisResult &result, double balance,
                  const Goal &goal);

  // Send user message to Grok and return Grok's response (or an error string).
  std::string respond(const std::string &userMessage) const;

private:
  SmartAnalyzer::AnalysisResult m_result;
  double m_balance{0.0};
  Goal m_goal;

  std::string buildSystemContext() const;
  static std::string categoryToString(SpendingCategory category);
};
