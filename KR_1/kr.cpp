// Visual Studio на windows, g++ на Linux
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

const int TOTAL_ATTEMPTS = 3;

class BankAccount {
private:
    double balance;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<double> operations;
    std::deque<std::pair<int, int>> delaydOperations;
    size_t maxQueueSize;

public:

    BankAccount(double initialBalance, size_t maxQueueSize) : balance(initialBalance), maxQueueSize(maxQueueSize) {}

    

    void changeBalance(double amount) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return operations.size() < maxQueueSize; });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        operations.push(amount);
        cv.notify_one();
    }

    void processOperations() {
        while (true) {
            double amount;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return !operations.empty(); });
                amount = operations.front();
                operations.pop();
                cv.notify_one();
            }
            double newBalance = balance + amount;
            if (newBalance >= 0) {
                std::cout << "было=" << balance << ", операция " << (amount >= 0 ? '+' : '-') << std::abs(amount) << ", стало=" << newBalance << std::endl;
                balance = newBalance;
            }
            //else { changeBalance(amount); }
            
            else { 
                //auto p = new std::pair<int, int>(0, amount);
                delaydOperations.push_back(*new std::pair<int, int>(0, amount));
            }
        }
    }

    void processDelaydOperations() {
        
        while (true) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            double amount;
            {
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] { return !delaydOperations.empty(); });
                    amount = delaydOperations.front().second;

                    cv.notify_one();

                }

                if (delaydOperations.front().first < TOTAL_ATTEMPTS) {
                    double newBalance = balance + amount;
                
                    if (newBalance >= 0) {
                        std::unique_lock<std::mutex> lock(mtx);
                        std::cout << "было=" << balance << ", операция " << (amount >= 0 ? '+' : '-') << std::abs(amount) << ", стало=" << newBalance << std::endl;
                        balance = newBalance;
                        delaydOperations.pop_front();
                    }
                    else {
                        auto p = delaydOperations.front(); 
                        delaydOperations.pop_front();
                        p.first = p.first++;
                        delaydOperations.push_front(p);
                    }
                }
                else { 
                    std::unique_lock<std::mutex> lock(mtx);
                    std::cout << "Отклоненная операция: " << amount << std::endl;
                    delaydOperations.pop_front();
                }
                
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
        }
    }

    double getBalance() {
        std::unique_lock<std::mutex> lock(mtx);
        return balance;
    }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    BankAccount account(0, 100); 

  
    std::thread scenario1([&]() {
        account.changeBalance(-100);
        account.changeBalance(50);
        account.changeBalance(50);
        account.changeBalance(50);
        account.changeBalance(-1000);
        account.changeBalance(-1000);
        account.changeBalance(-1000);        
        });

    std::thread scenario2([&]() {
        account.changeBalance(-200);
        account.changeBalance(-20);
        account.changeBalance(80);
        account.changeBalance(80);
        account.changeBalance(30);
        account.changeBalance(30);
        });

    std::thread processingThread([&]() {
        account.processOperations();
        });

    std::thread processingDelaydThread([&]() {
        account.processDelaydOperations();
        });



    scenario1.join();
    scenario2.join();
    
    processingThread.detach();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    processingDelaydThread.detach();


    std::this_thread::sleep_for(std::chrono::milliseconds(500));  
    std::cout << "Final balance: " << account.getBalance() << std::endl;

    return 0;
}
