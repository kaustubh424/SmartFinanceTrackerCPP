#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
using namespace std;

struct Expense {
    string date, category, description;
    float amount;
};

class FinanceTracker {
    vector<Expense> expenses;
    float monthlyBudget = 0.0;

    Expense parseCSVLine(const string &line) {
        stringstream ss(line);
        string date, category, amountStr, description;
        getline(ss, date, ',');
        getline(ss, category, ',');
        getline(ss, amountStr, ',');
        getline(ss, description);  // Remaining part is description
        return {date, category, description, stof(amountStr)};
    }

public:
    void loadFromFile() {
        expenses.clear();
        ifstream fin("expenses.csv");
        string line;
        while (getline(fin, line)) {
            try {
                Expense e = parseCSVLine(line);
                expenses.push_back(e);
            } catch (...) {
                // Skip bad lines
            }
        }
        fin.close();
    }

    void addExpense() {
        Expense e;
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> e.date;
        cout << "Enter Category (e.g., Food, Travel): ";
        cin >> e.category;
        cout << "Enter Amount: ";
        cin >> e.amount;
        cin.ignore();  // clear buffer
        cout << "Enter Description: ";
        getline(cin, e.description);

        ofstream fout("expenses.csv", ios::app);
        fout << e.date << "," << e.category << "," << e.amount << "," << e.description << "\n";
        fout.close();
        cout << "✅ Expense added successfully!\n";
    }

    void viewExpenses() {
        loadFromFile();
        if (expenses.empty()) {
            cout << "No expenses found.\n";
            return;
        }

        cout << "\n📋 All Expenses:\n";
        cout << left << setw(12) << "Date" << setw(15) << "Category"
             << setw(10) << "Amount" << "Description\n";
        cout << "---------------------------------------------------------------\n";

        for (const auto &e : expenses) {
            cout << left << setw(12) << e.date << setw(15) << e.category
                 << setw(10) << fixed << setprecision(2) << e.amount << e.description << "\n";
        }
    }

    void summaryByCategory() {
        loadFromFile();
        map<string, float> categoryTotals;
        for (const auto &e : expenses) {
            categoryTotals[e.category] += e.amount;
        }

        if (categoryTotals.empty()) {
            cout << "No expenses found.\n";
            return;
        }

        cout << "\n📊 Category-wise Summary:\n";
        for (const auto &pair : categoryTotals) {
            cout << left << setw(15) << pair.first << ": ₹" << fixed << setprecision(2) << pair.second << "\n";
        }
    }

    void setMonthlyBudget() {
        cout << "Enter your monthly budget (₹): ";
        cin >> monthlyBudget;
        cout << "✅ Budget set to ₹" << fixed << setprecision(2) << monthlyBudget << "\n";
    }

    void budgetAnalysis() {
        loadFromFile();
        float total = 0;
        for (const auto &e : expenses) total += e.amount;

        cout << "\n💡 Budget Analysis:\n";
        cout << "Total Spent: ₹" << fixed << setprecision(2) << total << "\n";

        if (monthlyBudget > 0) {
            if (total > monthlyBudget)
                cout << "⚠️ Over budget by ₹" << total - monthlyBudget << "\n";
            else
                cout << "✅ Within budget. ₹" << monthlyBudget - total << " remaining.\n";
        } else {
            cout << "ℹ️ No budget set yet.\n";
        }
    }
};

int main() {
    FinanceTracker ft;
    int choice;
    do {
        cout << "\n====== Smart Finance Tracker ======\n";
        cout << "1. Add Expense\n";
        cout << "2. View Expenses\n";
        cout << "3. Category-wise Summary\n";
        cout << "4. Set Monthly Budget\n";
        cout << "5. Budget Analysis\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: ft.addExpense(); break;
            case 2: ft.viewExpenses(); break;
            case 3: ft.summaryByCategory(); break;
            case 4: ft.setMonthlyBudget(); break;
            case 5: ft.budgetAnalysis(); break;
            case 6: cout << "👋 Exiting. Thank you!\n"; break;
            default: cout << "❗Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
