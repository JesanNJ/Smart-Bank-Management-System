# Smart-Driven Bank Management System

An advanced, console-based C++ banking application that integrates core financial management tools with a **Smart Analyzer**. It allows users to track their spending habits, set monthly budgets, and define long-term savings goals while dynamically assessing their financial health.

## Key Features
* **Smart Financial Analyzer:** Automatically analyzes spending behavior across different categories, evaluates the user's savings ratio, and scores overall financial health (0-100).
* **Budget Tracking:** Set category-based and monthly limits to receive early warnings when risking overspending or living paycheck-to-paycheck.
* **Goal Simulation:** Track custom savings goals (e.g., "Emergency Fund", "Vacation") mapped against monthly progress and timeline estimates.
* **Automated Reporting:** Generates a real-time spending trend analysis (using percentage shares) and simplified monthly income/expense flows.
* **Granular Transaction Logging:** Stores a timestamped, highly granular breakdown of money-movement, mapping every withdrawal to 11 specific `SpendingCategory`s (e.g. Housing, Dining, Healthcare, Entertainment).
* **Pre-Populated Database:** Deploys with 5 diverse dummy profiles (75+ structured transactions out of the box) spanning distinct financial risks and lifestyles to immediately test the Analyzer functionality.

## Core Technologies
* C++17 (Modern Standards, STL mapping, and memory safety)
* Command Line / Console Architecture
* CMake Build System

---

## 🚀 Getting Started

To compile the application using CMake:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
Following the build step, run the executable binary generated within the build folder (e.g., `./bank_management`).

---

## 🔑 Demo Credentials

To test the system immediately, the local persistent database is pre-loaded with comprehensive test data mimicking real-world behaviors. You can log into any of these accounts by selecting **Option 2: User Login** at the start screen.

**NOTE:** The login flow requires the **Account No** and the **Password**.

### Admin Dashboard Access
Use the Admin Dashboard to mass-view all active accounts and cross-reference over 75+ generated transactions. Select **Option 3: Admin Login**.
* **Username:** `admin`
* **Password:** `admin123`

### Pre-Registered Users

| Profile Name | Acc No.   | Password | Expected Financial Status |
| ------------ | --------- | -------- | ------------------------- |
| **John**     | `AC000001` | `password`| **Healthy.** Exceeds 20% savings margin. Meets all budgets easily. |
| **Alice**    | `AC000002` | `1234`    | **Needs Help.** Overspending heavily on discretionary items (Music Festivals, High-End Dining). Has very low savings and breaks her set budget limit. |
| **Bob**      | `AC000003` | `pass`    | **High Risk.** Living entirely paycheck-to-paycheck. Huge volume of micro-transactions (Fast food, subscriptions, taking out cash) with exactly $0 in deposits towards savings. |
| **Clara**    | `AC000004` | `admin123`| **Strict Budgeter.** Perfect 20% index fund strategy that matches her exact utility limits. Has an organized setup for her House Renovation goal. |
| **David**    | `AC000005` | `rich`    | **Wealthy.** Very large inputs coupled with extreme output variables. Michelin Star dining and luxury living. Generates huge savings margins anyway. |
