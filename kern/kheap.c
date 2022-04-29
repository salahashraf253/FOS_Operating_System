#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

void *nextFit(unsigned int size);
struct KernalHeapPages{
	uint32 startAddress;
	uint32 size;
}kernalHeapPages[(KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE];
uint32 kernalHeapFirstFreeVirtualAddress = KERNEL_HEAP_START;

void* kmalloc(unsigned int size)
{
//	cprintf("Hello,In kmalloc test\n");
	//TODO: [PROJECT 2022 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//===================================================================================================
	//NOTE: Allocation using NEXTFIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	/* start of NEXTFIT strategy  */
//	cprintf("Size before rounding: %d\n",size);
	size=ROUNDUP(size,PAGE_SIZE);	//from the document
//	cprintf("Size after rounding: %d\n",size);
	uint32 startAddress =(uint32)nextFit(size);
	if(!startAddress){
		return NULL;	//there no enough space
	}
	uint32 tmpstart = startAddress;
	struct Frame_Info *ptr_frame_info=NULL;
	for(uint32 tmpSize =0;tmpSize<size ;tmpSize+=PAGE_SIZE){
		allocate_frame(&ptr_frame_info);
		map_frame(ptr_page_directory, ptr_frame_info, (void*)tmpstart, PERM_PRESENT | PERM_WRITEABLE);
		kernalHeapPages[(tmpstart - KERNEL_HEAP_START) / PAGE_SIZE].startAddress = startAddress;
		kernalHeapPages[(tmpstart - KERNEL_HEAP_START) / PAGE_SIZE].size = size;
		tmpstart += PAGE_SIZE;
	}
	kernalHeapFirstFreeVirtualAddress = tmpstart;
	return (void*)startAddress;
	/*End of NEXTFIT strategy*/
	//===================================================================================================

	//TODO: [PROJECT 2022 - BONUS1] Implement a Kernel allocation strategy
	// Instead of the Next allocation/deallocation, implement
	// BEST FIT strategy
	// use "isKHeapPlacementStrategyBESTFIT() ..."
	// and "isKHeapPlacementStrategyNEXTFIT() ..."
	//functions to check the current strategy
	//change this "return" according to your answer
	return NULL;
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [2] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code
	panic("kfree() is not implemented yet...!!");

	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2022 - [3] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
	panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer

	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2022 - [4] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code
	panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer
	return 0;
}

void *nextFit(uint32 size){
//	cprintf("You are testing the next fit\n");
	uint32 startAddress = kernalHeapFirstFreeVirtualAddress;
	uint32 freeSize = 0;
	uint32 End = kernalHeapFirstFreeVirtualAddress;
	for(startAddress+=PAGE_SIZE ; startAddress< KERNEL_HEAP_MAX;startAddress+=PAGE_SIZE){
		if (kernalHeapPages[(startAddress - KERNEL_HEAP_START) / PAGE_SIZE].size == 0){
			freeSize += PAGE_SIZE;
			if (freeSize == size){
				startAddress -= size;
				break;
			}
		}
	}
	if (freeSize != size){
		return 0;
	}
	return (void*)startAddress;
}
