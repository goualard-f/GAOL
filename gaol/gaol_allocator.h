/***************************************************************************
 *   Copyright (C) 2008 by Frederic Goualard                               *
 *   Frederic.Goualard@univ-nantes.fr                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**
	\name gaol_allocator.h

	\author Frederic Goualard

	STL allocator that can be used to allocate memory that
	is 16-bytes aligned. This is paramount for some types such as
	the ones that use SSE2 registers.
*/

#ifndef __gaol_allocator_h__
#define __gaol_allocator_h__

#include <limits>
#include <new>
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#include <stdlib.h>

namespace gaol {

template <class T>
class aligned_allocator
{
	public:
		// type definitions
		typedef T        value_type;
		typedef T*       pointer;
		typedef const T* const_pointer;
		typedef T&       reference;
		typedef const T& const_reference;
		typedef std::size_t    size_type;
		typedef std::ptrdiff_t difference_type;

		template <class U>
		struct rebind
		{
			typedef aligned_allocator<U> other;
		};

		pointer address ( reference value ) const
		{
			return &value;
		}

		const_pointer address ( const_reference value ) const
		{
			return &value;
		}

		aligned_allocator()
		{
		}

		aligned_allocator ( const aligned_allocator& )
		{
		}

		template <class U>
		aligned_allocator ( const aligned_allocator<U>& )
		{
		}

		~aligned_allocator()
		{
		}

		size_type max_size () const
		{
			return std::numeric_limits<std::size_t>::max() / sizeof ( T );
		}

		pointer allocate ( size_type num, const void* = 0 )
		{
			void *buf;
			if ( MEMALIGN( buf,16,num*sizeof ( value_type ) ) )   // Allocation error?
			{
				throw std::bad_alloc();
			}
			return pointer ( buf );
		}

		// initialize elements of allocated storage p with value value
		void construct ( pointer p, const T& value )
		{
			// initialize memory with placement new
			new ( ( void* ) p ) T ( value );
		}

		// destroy elements of initialized storage p
		void destroy ( pointer p )
		{
			// destroy objects by calling their destructor
			p->~T();
		}

		// deallocate storage p of deleted elements
		void deallocate ( pointer p, size_type num )
		{
			// print message and deallocate memory with global delete
			free((void*)p);
		}
};

} // namespace gaol
#endif // __gaol_allocator_h__
