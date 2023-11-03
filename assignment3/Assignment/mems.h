/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
struct sub_chain_node{
    int type; // 0 means allocated to process, 1 means hole
    int size;
    int virtual_address_start; 
    int virtual_address_end; 
    struct sub_chain_node *next;
    struct sub_chain_node *prev;
};

struct main_chain_node{
    int type; // 0 means allocated to process, 1 means hole
    int no_pages;
    int *memory_address;
    int virtual_address_start; 
    int virtual_address_end; 
    struct main_chain_node *next;
    struct main_chain_node *prev;
    struct sub_chain_node *sub_chain;
} ;


/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096


/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
struct main_chain_node *head;
int virtual_address;
void mems_init(){
    head=NULL;
    virtual_address=1000;
}


/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
    struct main_chain_node *curnode=head;
    struct main_chain_node *nextnode=curnode->next;
    while(curnode!=NULL){
        struct sub_chain_node *cursubnode=curnode->sub_chain;
        struct sub_chain_node *nextsubnode=cursubnode->next;
        while(cursubnode!=NULL){
            munmap(cursubnode,PAGE_SIZE);
            if(nextsubnode==NULL){
                break;
            }
            cursubnode=nextsubnode;
            nextsubnode=nextsubnode->next;
        }
        munmap(curnode->memory_address,curnode->no_pages*PAGE_SIZE);
        munmap(curnode,PAGE_SIZE);
        if(nextnode==NULL){
            break;
        }
        curnode=nextnode;
        nextnode=nextnode->next;
    }
    head=NULL;
}


/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 
void* mems_malloc(size_t size){
    long virtaul_adress_return;
    if(head==NULL){
        int no_of_pages;
        if(size%PAGE_SIZE==0){
            no_of_pages=(int)(size/PAGE_SIZE);
        }else{
            no_of_pages=(int)(size/PAGE_SIZE)+1;
        }
        struct main_chain_node *node=NULL;
        node=(struct main_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        int *allocted_mem=(int *)mmap(NULL,no_of_pages*PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(node==MAP_FAILED || allocted_mem==MAP_FAILED){
            printf("mmap failed");
        }else{
            head=(node);
            node->type=0;
            node->no_pages=no_of_pages;
            node->memory_address=allocted_mem;
            node->virtual_address_start=virtual_address;
            node->virtual_address_end=node->virtual_address_start+(no_of_pages*PAGE_SIZE)-1;
            node->next=NULL;
            node->prev=NULL;
            node->sub_chain=NULL;
            struct sub_chain_node *sub_node_process=NULL;
            virtual_address=node->virtual_address_end;
            // printf("node is : %d %d %p %d %d",node->type,node->no_pages,node->memory_address,node->virtual_address_start,node->virtual_address_end);
            sub_node_process=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
            if(sub_node_process==MAP_FAILED){
                printf("mmap failed");
            }else{
                sub_node_process->type=0;
                sub_node_process->size=size;
                sub_node_process->virtual_address_start=node->virtual_address_start;
                sub_node_process->virtual_address_end=sub_node_process->virtual_address_start+sub_node_process->size-1;
                sub_node_process->next=NULL;
                sub_node_process->prev=NULL;
                node->sub_chain=sub_node_process;
                virtaul_adress_return=sub_node_process->virtual_address_start;
                // printf("\n");
                // printf("sub_node_process is : %d %d %d %d",sub_node_process->type,sub_node_process->size,sub_node_process->virtual_address_start,sub_node_process->virtual_address_end);
            }
            if(node->no_pages*PAGE_SIZE-size>0){
                struct sub_chain_node *sub_node_hole=NULL;
                sub_node_hole=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                if(sub_node_hole==MAP_FAILED){
                    printf("mmap failed");
                }else{
                    sub_node_hole->type=1;
                    sub_node_hole->size=node->no_pages*PAGE_SIZE-size;
                    sub_node_hole->virtual_address_start=sub_node_process->virtual_address_end+1;
                    sub_node_hole->virtual_address_end=sub_node_hole->virtual_address_start+sub_node_hole->size-1;
                    sub_node_hole->next=NULL;
                    sub_node_hole->prev=sub_node_process;
                    sub_node_process->next=sub_node_hole;
                    // printf("\n");
                    // printf("sub_node_process is : %d %d %d %d\n",sub_node_hole->type,sub_node_hole->size,sub_node_hole->virtual_address_start,sub_node_hole->virtual_address_end);
                }
            }
            
        }

    }else{
        int found=0;
        struct main_chain_node *curnode=head;
        while(curnode!=NULL){
            struct sub_chain_node *cursubnode=curnode->sub_chain;
            while(cursubnode!=NULL){
                if(cursubnode->type==1 && cursubnode->size>=size){
                    virtaul_adress_return=cursubnode->virtual_address_start;
                    found=1;
                    if(cursubnode->size>size){
                        if(cursubnode->next!=NULL){
                            
                            struct sub_chain_node *nextsubnode=cursubnode->next;
                            struct sub_chain_node *sub_node_new_hole=NULL;
                            sub_node_new_hole=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                            if(sub_node_new_hole==MAP_FAILED){
                                printf("mmap failed");
                            }else{
                                sub_node_new_hole->type=1;
                                cursubnode->type=0;
                                sub_node_new_hole->size=cursubnode->size-size;
                                cursubnode->size=size;
                                cursubnode->virtual_address_end=cursubnode->virtual_address_start+cursubnode->size-1;
                                sub_node_new_hole->virtual_address_start=cursubnode->virtual_address_end+1;
                                sub_node_new_hole->virtual_address_end=sub_node_new_hole->virtual_address_start+sub_node_new_hole->size-1;
                                cursubnode->next=sub_node_new_hole;
                                nextsubnode->prev=sub_node_new_hole;
                                sub_node_new_hole->prev=cursubnode;
                                sub_node_new_hole->next=nextsubnode;
                            }
                        }else{
                            struct sub_chain_node *sub_node_new_hole=NULL;
                            sub_node_new_hole=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                            if(sub_node_new_hole==MAP_FAILED){
                                printf("mmap failed");
                            }else{
                                sub_node_new_hole->type=1;
                                cursubnode->type=0;
                                sub_node_new_hole->size=cursubnode->size-size;
                                cursubnode->size=size;
                                cursubnode->virtual_address_end=cursubnode->virtual_address_start+cursubnode->size-1;
                                sub_node_new_hole->virtual_address_start=cursubnode->virtual_address_end+1;
                                sub_node_new_hole->virtual_address_end=sub_node_new_hole->virtual_address_start+sub_node_new_hole->size-1;
                                cursubnode->next=sub_node_new_hole;
                                sub_node_new_hole->prev=cursubnode;
                                sub_node_new_hole->next=NULL;
                            }

                        }
                    }else{
                        cursubnode->type=0;
                    }
                    break;
                }else{
                    cursubnode=cursubnode->next;
                }
            }
            if(found==1){
                break;
            }
            if(curnode->next==NULL){
                break;
            }
            curnode=curnode->next;
        }
        if(found==0){
            int no_of_pages;
            if(size%PAGE_SIZE==0){
                no_of_pages=(int)(size/PAGE_SIZE);
            }else{
                no_of_pages=(int)(size/PAGE_SIZE)+1;
            }
            struct main_chain_node *node=NULL;
            node=(struct main_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
            int *allocted_mem=(int *)mmap(NULL,no_of_pages*PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if(node==MAP_FAILED || allocted_mem==MAP_FAILED){
                printf("mmap failed");
            }else{
                curnode->next=(node);
                node->type=0;
                node->no_pages=no_of_pages;
                node->memory_address=allocted_mem;
                node->virtual_address_start=virtual_address+1;
                node->virtual_address_end=node->virtual_address_start+(no_of_pages*PAGE_SIZE)-1;
                node->next=NULL;
                node->prev=curnode;
                node->sub_chain=NULL;
                virtual_address=node->virtual_address_end;
                struct sub_chain_node *sub_node_process=NULL;
                // printf("node is : %d %d %p %d %d",node->type,node->no_pages,node->memory_address,node->virtual_address_start,node->virtual_address_end);
                sub_node_process=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                if(sub_node_process==MAP_FAILED){
                    printf("mmap failed");
                }else{
                    sub_node_process->type=0;
                    sub_node_process->size=size;
                    sub_node_process->virtual_address_start=node->virtual_address_start;
                    sub_node_process->virtual_address_end=sub_node_process->virtual_address_start+sub_node_process->size-1;
                    sub_node_process->next=NULL;
                    sub_node_process->prev=NULL;
                    node->sub_chain=sub_node_process;
                    virtaul_adress_return=sub_node_process->virtual_address_start;
                    // printf("\n");
                    // printf("sub_node_process is : %d %d %d %d",sub_node_process->type,sub_node_process->size,sub_node_process->virtual_address_start,sub_node_process->virtual_address_end);

                }
                if(node->no_pages*PAGE_SIZE-size>0){

                    struct sub_chain_node *sub_node_hole=NULL;
                    sub_node_hole=(struct sub_chain_node *)(mmap(NULL,PAGE_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
                    if(sub_node_hole==MAP_FAILED){
                        printf("mmap failed");
                    }else{
                        sub_node_hole->type=1;
                        sub_node_hole->size=node->no_pages*PAGE_SIZE-size;
                        sub_node_hole->virtual_address_start=sub_node_process->virtual_address_end+1;
                        sub_node_hole->virtual_address_end=sub_node_hole->virtual_address_start+sub_node_hole->size-1;
                        sub_node_hole->next=NULL;
                        sub_node_hole->prev=sub_node_process;
                        sub_node_process->next=sub_node_hole;
                        // printf("\n");
                        // printf("sub_node_process is : %d %d %d %d\n",sub_node_hole->type,sub_node_hole->size,sub_node_hole->virtual_address_start,sub_node_hole->virtual_address_end);
                    }
                }
                
            }
        }

    }
    return (void*)(virtaul_adress_return);

}


/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats(){
    printf("---- MeMS SYSTEM STATS ----\n");
    struct main_chain_node *curnode=head;
    int pages_used=0;
    int space_unused=0;
    int main_chain_length=0;
    while(curnode!=NULL){
        // printf("memroy address:%d\n",curnode->memory_address);
        printf("MAIN[%d:%d]-> ",curnode->virtual_address_start,curnode->virtual_address_end);
        pages_used+=curnode->no_pages;
        main_chain_length+=1;
        // printf("node is : type:%d, no of pages:%d, memory address:%p, v_start:%d, v_end:%d\n",curnode->type,curnode->no_pages,curnode->memory_address,curnode->virtual_address_start,curnode->virtual_address_end);
        struct sub_chain_node *cursubnode=curnode->sub_chain;
        while(cursubnode!=NULL){
            if(cursubnode->type==0){
                printf("P[%d:%d] <-> ",cursubnode->virtual_address_start,cursubnode->virtual_address_end);
                
            }else{
                printf("H[%d:%d] <-> ",cursubnode->virtual_address_start,cursubnode->virtual_address_end);
                space_unused+=cursubnode->size;
            }
            // printf("sub node is : type:%d, size:%d, v_start:%d, v_end:%d\n",cursubnode->type,cursubnode->size,cursubnode->virtual_address_start,cursubnode->virtual_address_end);
            cursubnode=cursubnode->next;
            if(cursubnode==NULL){
                printf("NULL\n");
            }
        }
        curnode=curnode->next;
    }
    printf("Pages used: %d\n", pages_used);
    printf("Space unused %d\n", space_unused);
    printf("Main chain Length: %d\n",main_chain_length);
    printf("Sub-chain Length array: [");
    curnode=head;
    while(curnode!=NULL){
        int sub_array_count=0;
        struct sub_chain_node *cursubnode=curnode->sub_chain;
        while(cursubnode!=NULL){
            cursubnode=cursubnode->next;
            sub_array_count+=1;
        }
        printf("%d, ",sub_array_count);
        curnode=curnode->next;
    }
    printf("]\n");

    printf("---------------------------\n");
}


/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void*v_ptr){
    long vir_memory_addr=(long)v_ptr;
    long phy_memory_addr;
    struct main_chain_node *curnode=head;
    while(curnode!=NULL){
        if(vir_memory_addr>=curnode->virtual_address_start && vir_memory_addr<curnode->virtual_address_end){
            phy_memory_addr=(long)curnode->memory_address+(vir_memory_addr-curnode->virtual_address_start);
            break;
        }
        curnode=curnode->next;
    }
    return (void *)(phy_memory_addr);
    // printf("%ld\n",phy_memory_addr);
}


/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr){
    long vir_memory_addr=(long)v_ptr;
    struct main_chain_node *curnode=head;
    int found=0;
    while(curnode!=NULL){
        struct sub_chain_node *cursubnode=curnode->sub_chain;
        if(vir_memory_addr>=curnode->virtual_address_start && vir_memory_addr<curnode->virtual_address_end){
            while(cursubnode!=NULL){
                if(vir_memory_addr==cursubnode->virtual_address_start){
                    cursubnode->type=1;
                    found=1;
                    break;
                }
                cursubnode=cursubnode->next;
            }
            if(found==1){
                break;
            }
        }
        curnode=curnode->next;
    }
    curnode=head;
    while(curnode!=NULL){
        struct sub_chain_node *cursubnode=curnode->sub_chain;
            while(cursubnode!=NULL){
                if(cursubnode->next==NULL){
                    break;
                }else{
                    struct sub_chain_node *nextsubnode=cursubnode->next;
                    if(cursubnode->type==1 && nextsubnode->type==1){
                        cursubnode->size+=nextsubnode->size;
                        cursubnode->virtual_address_end=nextsubnode->virtual_address_end;
                        cursubnode->next=nextsubnode->next;
                        if(nextsubnode->next!=NULL){
                            nextsubnode->next->prev=cursubnode;
                        }
                    }
                }
                cursubnode=cursubnode->next;
            }
            curnode=curnode->next;
        }
        
    }

