#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Synchro/Adaptor_synchro.hpp"

namespace aff3ct
{
namespace module
{
	template <typename T>
	runtime::Task& Adaptor_synchro<T>
	::operator[](const adp_synchro::tsk t)
	{
		return Module::operator[]((size_t)t);
	}
	template <typename T>
	runtime::Socket& Adaptor_synchro<T>
	::operator[](const adp_synchro::sck::adp_sck s)
	{
		return Module::operator[]((size_t)adp_synchro::tsk::synchroniser)[(size_t)s];
	}

	template <typename T>
	template <class A>
	void Adaptor_synchro<T>
	::synchroniser(std::vector<T,A>& inout, const int frame_id, const bool managed_memory)
	{
		(*this)[adp_synchro::sck::adp_sck::inout].bind(inout);
		(*this)[adp_synchro::tsk::synchroniser].exec(frame_id,managed_memory);
	}

	
}



}
