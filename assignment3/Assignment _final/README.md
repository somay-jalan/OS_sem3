# MeMS: Memory Management System [CSE231 OS Assignment 3]
[Documentation](https://docs.google.com/document/d/1Gs9kC3187lLrinvK1SueTc8dHCJ0QP43eRlrCRlXiCY/edit?usp=sharing)
---

### How to run the exmaple.c
After implementing functions in mems.h follow the below steps to run example.c file
```
$ make
$ ./example
```
---
### Format of mems_mems_print_stats
* For every Subchain in the free list print the data as follows 
```
MAIN[starting_mems_vitual_address:ending_mems_vitual_address] -> <HOLE or PROCESS>[starting_mems_vitual_address:ending_mems_vitual_address] <-> ..... <-> NULL
```
* After printing the whole freelist print the following stats
```
Page used: <Total Pages used by the user>
Space unused: <Total space mapped but not used by user>
Main Chain Length: <Length of the main chain>
Sub-chain Length array: <Array denoting the length of the subchains>
```


### Explanantion of code in mems.h
The code consists of 2 structs- 
sub_chain_node: This is the struct for the sub chain node of the free list.
main_chain_node: This is the struct for the main chain node of the free list.

Variables-

*head: This is the pointer to the start of the free list.
virtual address: This is the value of current virtual address of the free list.
*node_add_ptr_start: This is the pointer to the start of the Pages of the memory containing the free list.
*node_add_ptr: This is the pointer to the current memory address at which we are adding the node.

Explain of each function-

mems_init: Initializing head to NULL. Initializing virtual address = 1000. Initializing node_add_ptr_start to memory allocation of 5 PAGE_SIZE to accomadate the memory of the nodes and subnodes in the free list. Initializing node_add_ptr equal to node_add_ptr_start.

mems_finish: This function traverses through the free list. use munmap to unallocate the allocated memory to each node. Then it uses munmap to unallocate the memory alloacted to maintain the list of all the nodes in the free list.

mems_malloc: This function first checks if no main chain node is created then created a main chain node at the memory pointer of node_add_ptr and increasing node_add_ptr with value equal to size of main_chain_node and allocates memory using mmap with size = upper closest multiple of PAGE_SIZE and creates a process sub chain node in the main node at the memory pointer of node_add_ptr and increasing node_add_ptr with value equal to size of sub_chain_node for the size inputted in the function and makes a hole sub chain node at the memory pointer of node_add_ptr and increasing node_add_ptr with value equal to size of sub_chain_node for the size left in the main node.
If main chain node exists then if iterates through the free list to check if there is a hole size more than the size inputted, if there is a hole size more than the size inputted then it convertes that hole to process and changes the process size to inputted size and makes a hole of left memory from previous hole.
If there is no hole with size more than the size inputted the function goes to the last main chain node and created a new node like it is created in the first part of the function.

mems_print_stats: This function prints the stats of free list by iterating through it and keeping a track of pages used,space unused and main chain length and prints the information in required format.

mems_get: This fucntion iterates through the free list and find main chain node to which the inputted virtual address belongs to. Then it subtracts the starting virtual address of that main chain node from inputted virtual address and adds the physical address to that subtracted value to get the final physical address.

mems_free: This function iterates through the free list to find the sub chain node whose virtual starting address is equal to the inputted value and changes its type from process to hole.
Then it iterates through the free list to find adjacent holes in the list and add those holes to get a single big hole.


* Analyse the output of the example.c file below to get a better understanding of the output
---
### Example Output (already given in the assignment not our output)
PAGE_SIZE= 4096 Bytes

Starting MeMS Virtual Address= 1000

![Example Output](example_output.jpg)