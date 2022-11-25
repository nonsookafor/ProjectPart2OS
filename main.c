#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "BENSCHILLIBOWL.h"

// Feel free to play with these numbers! This is a great way to
// test your implementation.
#define BENSCHILLIBOWL_SIZE 10
#define NUM_CUSTOMERS 3
#define NUM_COOKS 10
#define ORDERS_PER_CUSTOMER 3
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ORDERS_PER_CUSTOMER

// Global variable for the restaurant.
BENSCHILLIBOWL *bcb;


/**
 * Thread funtion that represents a customer. A customer should:
 *  - allocate space (memory) for an order.
 *  - select a menu item.
 *  - populate the order with their menu item and their customer ID.
 *  - add their order to the restaurant.
 */
void* BENSCHILLIBOWLCustomer(void* tid) {
		pthread_mutex_lock(&bcb->mutex);
    int customer_id = (int)(long) tid;
		Order *order = (Order*) malloc(sizeof(Order));
		MenuItem myMenuItem = PickRandomMenuItem();
		order->menu_item = myMenuItem;
		order->customer_id = customer_id;
		order->next = NULL;
		printf("Order %s created;\n", order->menu_item);
		AddOrder(bcb, order);
		pthread_mutex_unlock(&bcb->mutex);
	return NULL;
}

/**
 * Thread function that represents a cook in the restaurant. A cook should:
 *  - get an order from the restaurant.
 *  - if the order is valid, it should fulfill the order, and then
 *    free the space taken by the order.
 * The cook should take orders from the restaurants until it does not
 * receive an order.
 */
void* BENSCHILLIBOWLCook(void* tid) {
		pthread_mutex_lock(&bcb->mutex);
    int cook_id = (int)(long) tid;
		int orders_fulfilled = 0;

		Order* myOrder = GetOrder(bcb);
		printf("%s\n", myOrder->menu_item);

		if (myOrder) {
			orders_fulfilled += 1;
			free(myOrder);
		}

		printf("Cook #%d fulfilled %d orders\n", cook_id, orders_fulfilled);
		pthread_mutex_unlock(&bcb->mutex);
		return NULL;
}

/**
 * Runs when the program begins executing. This program should:
 *  - open the restaurant
 *  - create customers and cooks
 *  - wait for all customers and cooks to be done
 *  - close the restaurant.
 */
int main() {
		printf("%s\n", PickRandomMenuItem());
		bcb = OpenRestaurant(BENSCHILLIBOWL_SIZE, EXPECTED_NUM_ORDERS);
		pthread_t threads[2];
		// int tid = 1;
		for (int i = 0; i < 2; i++) {
        if (i == 0) {
					pthread_create(&threads[i], NULL, BENSCHILLIBOWLCustomer, NULL);
				}
        else {
					sleep(1);
					pthread_create(&threads[i], NULL, BENSCHILLIBOWLCook, NULL);
				}
    }


		for (int i = 0; i < 2; i++) {
			pthread_join(threads[i], NULL);
		}

		CloseRestaurant(bcb);
    return 0;
}