#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

void randomNumber() {
	while (true) {
		cout << rand() << endl;
		this_thread::sleep_for(chrono::milliseconds(3000));
	}

}

void invertString() {
	while (true) {
		string s;
		cin >> s;
		std::reverse(s.begin(), s.end());
		cout << s << endl;
	}
}

void signal_handler(int signal) {
	cout << "Signal: " << signal << endl;

	exit(signal);
}

int main() {
	setlocale(LC_ALL, "ru");

	thread th1(randomNumber);
	thread th2(invertString);

	std::signal(SIGINT, signal_handler);

	

	th1.join();
	th2.join();

	return 0;
}
