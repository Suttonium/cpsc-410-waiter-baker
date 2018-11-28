/**
 * Authors: Chris Hurt & Austin Sutton
 * Participation: 50% & 50%
 */

#include <string>
#include "stdlib.h"
#include "../includes/externs.h"
#include "../includes/waiter.h"
#include "../includes/logger.h"

using namespace std;


Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
	ORDER order;
	while (getNext(order) == SUCCESS) {

		// Lock order in queue for async
		std::unique_lock<std::mutex> lk(mutex_order_inQ);
		// give queue an order
		order_in_Q.push(order);
		lk.unlock();
		// notify bakers
		cv_order_inQ.notify_all();
	}

	// lock once more
	std::unique_lock<std::mutex> lk(mutex_order_inQ);
	// tell bakers that waiter is finished
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

