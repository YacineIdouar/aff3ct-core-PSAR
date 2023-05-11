/*!
 * \file
 * \brief Class module::Adaptor_synchro.
 */
#ifndef ADAPTOR_SYNCHRO_HPP
#define ADAPTOR_SYNCHRO_HPP

#include <cstdint>
#include <vector>


#include "Runtime/Task/Task.hpp"
#include "Runtime/Socket/Socket.hpp"
#include "Module/Module.hpp"
#include "Module/Adaptor/Adaptor.hpp"


namespace aff3ct
{
namespace module
{
	namespace adp_synchro
	{
		enum class tsk : size_t { synchroniser, SIZE };

		namespace sck
		{
			enum class adp_sck : size_t {inout,status};
		}
	}

template <typename T = int>
class Adaptor_synchro : public Module
{

protected:
	const size_t n_elmts;
	size_t ns;
	module::Adaptor* adp_to_sync;

public:
	inline runtime::Task&   operator[](const adp_synchro::tsk         t);
	inline runtime::Socket& operator[](const adp_synchro::sck::adp_sck s);

public:
	Adaptor_synchro(const size_t n_elmts, const size_t ns = 0);
	virtual ~Adaptor_synchro() = default;
	virtual Adaptor_synchro* clone() const;


	size_t get_ns() const;
	void set_ns(const size_t ns);
	size_t get_n_elmts() const;
	void set_adaptor(const module::Adaptor* adp_to_sync);

	template <class A = std::allocator<T>>
	void synchroniser(std::vector<T,A>& inout,
	               const int frame_id = -1,
	               const bool managed_memory = true);

	void synchroniser(T* inout,
	               const int frame_id = -1,
	               const bool managed_memory = true);	

	protected : 
	void _synchroniser();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Synchro/Adaptor_synchro.hxx"
#endif

#endif /* ADAPTOR_SYNCHRO_HPP_ */
