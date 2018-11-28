/**
 * Authors: Chris Hurt & Austin Sutton
 * Participation: 50% & 50%
 */

#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/box.h"
#include <math.h>
#include "../includes/logger.h"
#include "../includes/waiter.h"

using namespace std;

Baker::Baker(int id) :
        id(id) {
}

Baker::~Baker() {
}

void Baker::bake_and_box(ORDER &anOrder) {
    // number of boxes for this order
    int num_boxes = (anOrder.number_donuts / 12) + 1;

    int nut_counter = anOrder.number_donuts;
    for (unsigned int i = 0; i < num_boxes; i++) {
        Box muh_kewl_box;
        DONUT nut;
        // keep adding donuts to box till full or no more donuts
        while (nut_counter > 0 && muh_kewl_box.addDonut(nut)) {
            nut_counter--;
        }
        // push box onto boxes vector
        anOrder.boxes.push_back(muh_kewl_box);
    }
}

void Baker::beBaker() {
    // don't do anything until there are orders or the waiter is finished
    while (!b_WaiterIsFinished || !order_in_Q.empty()) {
        // unique lock that waits on condition variable
        unique_lock<mutex> lk(mutex_order_inQ);

        // wait till there are orders and waiter is finished
        while (!b_WaiterIsFinished && order_in_Q.empty()) {
            cv_order_inQ.wait(lk);
        }

        // if orders then bake and box donuts
        if (!order_in_Q.empty()) {
            // Box order
            bake_and_box(order_in_Q.front());

            // second lock so out vector is async
            unique_lock<mutex> lk2(mutex_order_outQ);
            order_out_Vector.push_back(order_in_Q.front());
            lk2.unlock();

            order_in_Q.pop();

            // unlock and notify rest of bakers
            lk.unlock();
            cv_order_inQ.notify_all();
        }
    }
}
