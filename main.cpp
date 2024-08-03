#include "CashFlowMinimizer.h"
#include <iostream>
#include <algorithm>

void CashFlowMinimizer::addPerson(const Person &person) {
    people.push_back(person);
}

void CashFlowMinimizer::addTransaction(const Transaction &transaction) {
    for (auto &person : people) {
        if (person.getName() == transaction.getPayer()) {
            person.updateNetAmount(-transaction.getAmount());
        } else if (person.getName() == transaction.getPayee()) {
            person.updateNetAmount(transaction.getAmount());
        }
    }
}

int CashFlowMinimizer::getMinIndex() const {
    int minIndex = 0;
    for (int i = 1; i < people.size(); i++) {
        if (people[i].getNetAmount() < people[minIndex].getNetAmount()) {
            minIndex = i;
        }
    }
    return minIndex;
}

int CashFlowMinimizer::getMaxIndex() const {
    int maxIndex = 0;
    for (int i = 1; i < people.size(); i++) {
        if (people[i].getNetAmount() > people[maxIndex].getNetAmount()) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void CashFlowMinimizer::settleDebts() {
    int minIndex = getMinIndex();
    int maxIndex = getMaxIndex();

    if (people[minIndex].getNetAmount() == 0 && people[maxIndex].getNetAmount() == 0) {
        return;
    }

    int minAmount = std::min(-people[minIndex].getNetAmount(), people[maxIndex].getNetAmount());

    people[minIndex].updateNetAmount(minAmount);
    people[maxIndex].updateNetAmount(-minAmount);

    transactions.emplace_back(people[maxIndex].getName(), people[minIndex].getName(), minAmount);

    settleDebts();
}

void CashFlowMinimizer::minimizeCashFlow() {
    settleDebts();
}

void CashFlowMinimizer::printTransactions() const {
    std::cout << "The transactions for minimum cash flow are as follows:\n";
    for (const auto &transaction : transactions) {
        std::cout << transaction.getPayer() << " pays Rs " << transaction.getAmount() << " to " << transaction.getPayee() << "\n";
    }
}
