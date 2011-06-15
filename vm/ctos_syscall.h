/* The Mach syscalls are in <mach/syscall_sw.h>.  */
#ifndef CTOS_SYSCALL_H
#define CTOS_SYSCALL_H
#include <mach.h>

/*
 *	vm_truncate -
 *	Readjust the size of buffer starting at "address" to "size" in virtual address space of 
 *	task "target_map". The "size" rounds to virtual page boundaries.
 *
 */

int vm_truncate( 
	mach_port_t 	target_map,
	vm_offset_t 	address,
	vm_size_t 	size)
{
	return	syscall(-67,
			target_map,
			address,
			size);
}

/*
 *	ctos_vm_truncate -
 *	Readjust the size of buffer starting at "address" to "size" in current task's address 	
 *	space . The "size" rounds to virtual page boundaries.
 *
 */

int ctos_vm_truncate(vm_offset_t address, vm_size_t size)
{
	return vm_truncate(mach_task_self(), address, size);
}

/*
 * vm_remap -
 * Remap a range of memory from one task into another,
 * to another address range within the same task, or
 * over top of itself (with altered permissions and/or
 * as an in-place copy of itself).
 *
 * The addressability of the source and target address
 * range is limited by the size of vm_address_t (in the
 * kernel context).
 */

int vm_remap(
	mach_port_t  	target_map,
	vm_offset_t 	*address,
	vm_size_t	size,
	vm_offset_t	mask,
	boolean_t	anywhere,
	mach_port_t	src_map,
	vm_offset_t	memory_address,
	boolean_t	copy,
	vm_prot_t	*cur_protection,
	vm_prot_t	*max_protection,
	vm_inherit_t	inheritance)
{
	return syscall (-63,
			target_map,
			address,
			size,
			mask,
			anywhere,
			src_map,
			memory_address,
			copy,
			cur_protection,
			max_protection,
			inheritance);
}

/*
 * vm_remap -
 * Remap a range of memory from current task into another,
 * to another address range within the same task, or
 * over top of itself (with altered permissions and/or
 * as an in-place copy of itself).
 *
 * The addressability of the source and target address
 * range is limited by the size of vm_address_t (in the
 * kernel context).
 * Return the the start address of the range within target
 * task if success.
 */

void * ctos_vm_remap(
	vm_offset_t	src_address,
	mach_port_t	target_map,
	vm_size_t	size,
	vm_prot_t	protection)
{
	vm_offset_t target_address = NULL;
	vm_prot_t	max_protection = VM_PROT_ALL;
	int err;
	err =  vm_remap(target_map,
			&target_address,
			size,
			0,
			TRUE,
			mach_task_self(),
			src_address,
			FALSE,
			&protection,
			&max_protection,
			VM_INHERIT_NONE);
	if (err) 
		error(1, err, "vm_remap in function ctos_vm_remap");
	return (void *)target_address;
}
#endif /*CTOS_SYSCALL_H*/
