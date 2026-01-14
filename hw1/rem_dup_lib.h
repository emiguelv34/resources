#ifndef REMDUPLIB_H
#define REMDUPLIB_H


struct Item {
  Item(int v, Item* n) { val = v; next = n; }
  int val;
  Item* next;
};

// Deletes consecutive equal values from the list
void removeConsecutive(Item* head);

// Makes a copy of the items in the head1 and head2 lists
// concatenating the new head2 items to the end of the
// new head1 list items.
Item* concatenate(Item* head1, Item* head2);

#endif
