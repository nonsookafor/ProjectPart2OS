#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(BENSCHILLIBOWL* bcb);
bool IsFull(BENSCHILLIBOWL* bcb);
void AddOrderToBack(Order **orders, Order *order);

MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    srand(time(NULL));
    return BENSCHILLIBOWLMenu[rand() % (10)];
}

/* Allocate memory for the Restaurant, then create the mutex and condition 
variables needed to instantiate the Restaurant */

BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    printf("Restaurant is open!\n");
    BENSCHILLIBOWL *myRestaurant = (BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL));
    myRestaurant->max_size = max_size;
    myRestaurant->expected_num_orders = expected_num_orders;
    myRestaurant->orders = NULL;
    myRestaurant->current_size = 0;
    myRestaurant->next_order_number = 0;
    pthread_mutex_init(&myRestaurant->mutex, NULL);
    return myRestaurant;
}


/* check that the number of orders received is equal to the number handled
 (ie.fullfilled). Remember to deallocate your resources */

void CloseRestaurant(BENSCHILLIBOWL* bcb) {
    printf("Restaurant is closed!\n");
    if (bcb->orders_handled == bcb->expected_num_orders) {
      printf("Closing Restaurant");
      // destroy all the synchronization objects
      pthread_mutex_destroy(&bcb->mutex);
      free(bcb);
    }
}

/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    if (bcb->current_size != bcb->max_size) {
      if (bcb->orders == NULL) {
        bcb->orders = order;
      }
      else {
        Order* curr = bcb->orders;
        while (curr->next != NULL) {
          curr = curr->next;
        }
        curr->next = order;
      }
      // bcb->orders_handled ++;
      bcb->current_size ++;
    }
    return bcb->current_size;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
    // Wait until the restaurant is not empty
    if (bcb->orders != NULL) {
      Order* current = bcb->orders;
      bcb->orders = bcb->orders->next;
      return current;
    }
    else {
      printf("Hello cooks, there are no orders left");
    }
}    

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* bcb) {
  if (bcb->current_size == 0) {
    return true;
  }
  return false;
}

bool IsFull(BENSCHILLIBOWL* bcb) {
  if (bcb->current_size == bcb->max_size) {
    return true;
  }
  return false;
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {
  if (*orders == NULL) {
      *orders = order;
  }
  else {
    Order* curr = *orders;
    while (curr->next != NULL) {
      curr = curr->next;
    }
    curr->next = order;
  }
}


