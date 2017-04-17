/**
* Source: http://codereview.stackexchange.com/questions/51235/udp-network-server-client-for-gaming-using-boost-asio
* License: MIT
*/

#pragma once

#include <mutex>
#include <queue>
#include <list>

using namespace std;

template<typename T> class ThreadSafeQueue {
public:
	void push(T value) {
		unique_lock<mutex> lock(m);
		q.push(value);
	};

	T pop() {
		unique_lock<mutex> lock(m);
		T value;
		swap(value, q.front());
		q.pop();
		return value;
	};

	bool empty() {
		unique_lock<mutex> lock(m);
		return q.empty();
	}

private:
	mutex m;
	queue<T> q;
};