// -*- c++ -*-
/*
 * Copyright 2002, The libsigc++ Development Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#include <sigc++/functors/closure.h>

namespace sigc {
namespace functor {
namespace internal {

void* closure_rep::notify(void* p)
{
  closure_rep* self=(closure_rep*)p;
  self->call_ = 0;                    // Invalidate the closure.
  if (self->parent_)
    (self->cleanup_)(self->parent_);  // Notify the parent.
  return 0;
}

/*bool closure_base::empty() const // having this function not inline is killing performance !!!
{ 
  if (rep_ && !rep_->call_)
    {
      delete rep_;        // This is not strictly necessary here. I'm convinced that it is
      rep_ = 0;           // safe to wait for the destructor to delete the closure_rep. Martin.
    }
  return (rep_ == 0); 
}*/

bool closure_base::block(bool should_block)
{
  bool old = blocked_;
  blocked_ = should_block;
  return old;
}

// TODO: untested
closure_base& closure_base::operator=(const closure_base& cl)
{
  if (cl.rep_ == rep_) return *this;

  if (cl.empty())
    {
      disconnect();
      return *this;
    }

  closure_rep* new_rep_ = cl.rep_->dup();

  if (rep_)               // Silently exchange the closure_rep.
    {
      new_rep_->set_parent(rep_->parent_, rep_->cleanup_);
      delete rep_;
    }

  rep_ = new_rep_;

  return *this;
}

} /* namespace internal */
} /* namespace functor */
} /* sigc */
