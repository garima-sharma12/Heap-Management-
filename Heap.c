//BT20CSE040 GARIMA
//CPL ASSIGNMENT 3
//Write a program to implement heap management scheme. Implement the 'allocate' and 'free' functions.
// Ensure that adjacent free blocks are merged together.


#include<stdio.h>
#include<stdlib.h>
//Defining element for the heap storage
typedef struct HeapNode{
	int address;
	int size; 
	struct HeapNode* next;
	 
} heap_node;
//Defining pointers for the allocation and deallocation of heap elements
typedef struct HeapPtr{
	heap_node* free_ptr;
	heap_node* alloc_ptr;
}heap_ptr;
//Creating a heap node and initialising the struct elements for heap_node 
heap_node* CreateNode(int address,int size){
	heap_node* lptr=(heap_node*)malloc(sizeof(heap_node));  //creates memory of 'size' as initialised
	lptr->address=address; //points to start address
	lptr->size=size;
	lptr->next=NULL;
	return lptr;
}
//Separating a fixed amount of memory for heap storage
void InititaliseHeap(heap_ptr* nptr){
	nptr->alloc_ptr=NULL;
	nptr->free_ptr=CreateNode(0,1024);
}


//Allocating heap memory using first fit 
int Allocate(heap_ptr* lptr,int size){
	heap_node* prev=NULL;
	heap_node* nptr=lptr->free_ptr; //pointer to unoccupied address
	int diff=1025; //since maximum difference is 1024
    int address;
    int flag=0; 

    while(nptr!=NULL)  //check and find address of the first continous block of memory that is unoccupied
    {
        if((nptr->size)>=size)   //nptr->size points to the size of the heap memory covered
        {
            flag=1;             //if size of heap memory found is more than or equal to required size 
            if((nptr->size)-size<diff)    //the difference is the size that is left to allocate from the heap
            {
                diff=(nptr->size)-size;
                address=nptr->address;   //addresses of the allocated nodes
            }
        }
        nptr=nptr->next;  //else increment pointer and go ahead
    }
    if (flag==0){
    	printf("Heap Memory is full!\n");
        address=-1;
	}
    else
    {
        int found=0;
        nptr=lptr->free_ptr;   //start address of free heap memory
        while(nptr!=NULL && found==0)     //while heap memory is available and is not allocated for the required size
        {
            if(nptr->address==address){
            	found=1; 
			}     
            else
            {
                prev=nptr;   //at the end stores second last node
                nptr=nptr->next;
            }
        }  
        if(nptr->size==size)   
        {

            if(prev==NULL){
            	lptr->free_ptr=NULL;
			}
            else{
            	prev->next=nptr->next;
			}
        }
        else
        {
            
            heap_node* temp=CreateNode(nptr->address,size);  //creating nodes and allocating them
            nptr->address= nptr->address+size;
            nptr->size=nptr->size-size;
            nptr=temp;
        }
        nptr->next=lptr->alloc_ptr;    
        lptr->alloc_ptr=nptr;
    }
   
    return address;
}



//Deallocating memory
void Deallocate(heap_ptr* lptr,int address)
{
    
    heap_node* nptr=lptr->alloc_ptr;   //alloc_ptr holds address of allocated memory 
	heap_node* prev=NULL;  
    int found=0;
    
    while(nptr!=NULL && found==0)
    {
      
        if(nptr->address==address){   //check if element exists at the given address in the heap
        	found=1;
		}
        
        else
        {
            prev=nptr;
            nptr=nptr->next;
        }
    }
    
    if(found==0){
    	printf("No element exits at address you entered\n");
	}
    
    else
    {
        if(prev==NULL)
        {
            lptr->alloc_ptr=nptr->next;  //if the address is the start of the heap memory
        }
        else
        {
            prev->next=nptr->next;   //merge the memory after the given address with the address before it
        }
        
        heap_node* temp;
        heap_node* tail;
		heap_node* middle=NULL;
		heap_node* ahead=lptr->free_ptr;
	    //freeing the memory
        int found2=0;
         while(ahead!=NULL && found2==0)
        {
             
            if (ahead->address>nptr->address){  //traversing through heap and checking if memory is available after the address of the element
            found2=1;
			}
            else
            {
                tail=middle;
                middle=ahead;
                ahead=ahead->next;
            }
        }
 //middle points location just before the given address       
        if(found2==1)
            {
                if(middle==NULL)  //the address is the starting address of heap memory
                {
                    if(nptr->address+nptr->size==ahead->address)
                    {
                        temp=CreateNode(nptr->address,ahead->size+nptr->size);
                        temp->next=ahead->next;
                        free(ahead);
                    }
                    else
                    {
                        temp=CreateNode(nptr->address,nptr->size);
                        temp->next=ahead;
                    }
                    lptr->free_ptr=temp;  //start freeing from the node temp
                }
                 else
                {
                    int flag1=0,flag2=0;
                
                    if(nptr->address==middle->address+middle->size){  // if tptr points to location just after the previous node
                    	 flag1=1;
					}
                   

                    if(nptr->address+nptr->size==ahead->address){    // the address of next node coincides with the address after the given node tptr
                    	 flag2=1;
					}
                   
                    
                    if(flag1==1 && flag2==1)
                    {
                        temp=CreateNode(middle->address,middle->size+nptr->size+ahead->size);  //create empty nodes for the same location and merge  
                        temp->next=ahead->next;
                        if(tail==NULL)
                        {
                            lptr->free_ptr=temp;   //starting address of heap
                        }
                        else
                        {
                            tail->next=temp;
                        }
                        free(middle); // delete the old nodes 
                        free(ahead);
                    }
                    else if(flag1==1)
                    {
                        temp=CreateNode(middle->address,middle->size+nptr->size);  //node at the address prior to the given address with additional space that we get after freeing the given address node tptr
                        temp->next=ahead;
                        if(tail==NULL)  //tail is the node before middle
                        {
                            lptr->free_ptr=temp;   //start of free memory of heap points to the new node created 
                        } 
                        else
                        {
                            tail->next=temp;
                        }
                        free(middle);
                    }
                    else if(flag2==1)   
                    {
                        temp=CreateNode(nptr->address,ahead->size+nptr->size);
                        temp->next=ahead->next;
                        free(ahead);
                        middle->next=temp;
                    }
                    else
                    {
                        temp=CreateNode(nptr->address,nptr->size);
                        temp->next=ahead;
                        middle->next=temp;
                    }
                }
            }
            else
            {
                    if(nptr->address==middle->address+middle->size)
                    {
                        temp=CreateNode(middle->address,middle->size+nptr->size);
                        tail->next=temp;
                        free(middle);
                    }
                    else
                    {
                        temp=CreateNode(nptr->address,nptr->size);
                        middle->next=temp;
                    }
                    temp->next=NULL;
            }
            
        
    }
   
}       


//Printing the details about the allocated heap memory
void Print(heap_node* lptr){
	if (lptr==NULL){
		printf("Heap is Empty");
	}
	else{
		heap_node* nptr=lptr;
		while(nptr!=NULL){
			printf("Size of the heap memory allocated in bits: %d\n",nptr->size);
			printf("Address of the allocated heap: %d\n",nptr->address);
			nptr=nptr->next;
		}	
	}
}


int main(){
	
	heap_ptr hp;
	InititaliseHeap(&hp);
	int option;
	int stop=1;
	int size;
	int address;

    printf("Enter the number from the following options:\n");
    printf("1. Allocate memory\n");
    printf("2. Deallocate memory\n");
    printf("3. Print the allocated memory list\n");
    printf("4. Print the list of deallocated memeory\n");
    printf("5. To exit\n");
    
   
    while(stop==1)
    {
    	printf("Select the option\n");
    	scanf("%d",&option);
    	if(option==1){
    		printf("Enter the number of bits you want to allocate:\n");
    		scanf("%d",&size);
 		address=Allocate(&hp,size);
    		if (address>=0){
    			printf("Address allocated is: %d\n",address);
			}
		}
    	
		else if (option==2){
			printf("Enter the address of the memory you want to free:\n");
			scanf("%d",&address);
			Deallocate(&hp,address);
		}
		else if (option==3){
			printf("Allocation list:\n");
			Print(hp.alloc_ptr);
			printf("\n");	
		}
		else if (option==4){
			printf("Free list:\n");
		    Print(hp.free_ptr);
		    printf("\n");
			
		}
		else{
			stop=0;
		}
	}	 		
	
	return 0;
}
