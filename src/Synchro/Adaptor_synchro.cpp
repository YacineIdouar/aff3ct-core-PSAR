#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Synchro/Adaptor_synchro.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Adaptor_synchro<T>
::Adaptor_synchro(const size_t n_elmts, const size_t ns)
: Module(), n_elmts(n_elmts), ns(ns)
{
	const std::string name = "Adaptor_synchro";
	this->set_name(name);
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("synchroniser");
	auto ps_inout = this->template create_socket_inout<T>(p, "inout", this->n_elmts);
	this->create_codelet(p, [ps_inout](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &synchro = static_cast<Adaptor_synchro&>(m);
		synchro._synchroniser();
		return runtime::status_t::SUCCESS;
	});
}

template <typename T>
Adaptor_synchro<T>* Adaptor_synchro<T>
::clone() const
{
	auto m = new Adaptor_synchro(*this);
	m->deep_copy(*this);
	m->adp_to_sync = this->adp_to_sync;
	return m;
}

template <typename T>
size_t Adaptor_synchro<T>
::get_ns() const
{
	return this->ns;
}

template <typename T>
size_t Adaptor_synchro<T>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename T>
void Adaptor_synchro<T>
::set_ns(const size_t ns)
{
	this->ns = ns;
}

template <typename T>
void Adaptor_synchro<T>
::set_adaptor(const module::Adaptor* adp_to_sync)
{
	this->adp_to_sync = adp_to_sync;
}

template <typename T>
void  Adaptor_synchro<T>
::synchroniser(T *inout, const int frame_id, const bool managed_memory)
{
	(*this)[adp_synchro::sck::adp_sck::inout].bind(inout);
	(*this)[adp_synchro::tsk::synchroniser].exec(frame_id,managed_memory);
}

template <typename T>
void  Adaptor_synchro<T>
::_synchroniser()
{
	this->adp_to_sync->wake_up_pusher();
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Adaptor_synchro<int8_t>;
template class aff3ct::module::Adaptor_synchro<uint8_t>;
template class aff3ct::module::Adaptor_synchro<int16_t>;
template class aff3ct::module::Adaptor_synchro<uint16_t>;
template class aff3ct::module::Adaptor_synchro<int32_t>;
template class aff3ct::module::Adaptor_synchro<uint32_t>;
template class aff3ct::module::Adaptor_synchro<int64_t>;
template class aff3ct::module::Adaptor_synchro<uint64_t>;
template class aff3ct::module::Adaptor_synchro<float>;
template class aff3ct::module::Adaptor_synchro<double>;
// ==================================================================================== explicit template instantiation
