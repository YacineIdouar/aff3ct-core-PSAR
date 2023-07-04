#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <fstream>
#include <algorithm>
#include <getopt.h>

#include <aff3ct-core.hpp>
using namespace aff3ct;
using namespace aff3ct::runtime;

int main(int argc, char** argv)
{
	option longopts[] = {
		{"n-threads", required_argument, NULL, 't'},
		{"n-inter-frames", required_argument, NULL, 'f'},
		{"sleep-time", required_argument, NULL, 's'},
		{"data-length", required_argument, NULL, 'd'},
		{"buffer-size", required_argument, NULL, 'u'},
		{"dot-filepath", required_argument, NULL, 'o'},
		{"in-filepath", required_argument, NULL, 'i'},
		{"out-filepath", required_argument, NULL, 'j'},
		{"copy-mode", no_argument, NULL, 'c'},
		{"print-stats", no_argument, NULL, 'p'},
		{"step-by-step", no_argument, NULL, 'b'},
		{"debug", no_argument, NULL, 'g'},
		{"force-sequence", no_argument, NULL, 'q'},
		{"active-waiting", no_argument, NULL, 'w'},
		{"help", no_argument, NULL, 'h'},
		{0}};

	size_t n_threads = std::thread::hardware_concurrency();
	size_t n_inter_frames = 1;
	size_t sleep_time_us = 5;
	size_t data_length = 16;
	size_t buffer_size = 64;
	std::string dot_filepath = "./dot_full_io";
	bool no_copy_mode = true;
	bool print_stats = false;
	bool step_by_step = false;
	bool debug = false;
	bool force_sequence = false;
	bool active_waiting = false;

	while (1)
	{
		const int opt = getopt_long(argc, argv, "t:f:s:d:u:o:i:j:cpbgqwh", longopts, 0);
		if (opt == -1)
			break;
		switch (opt)
		{
			case 't':
				n_threads = atoi(optarg);
				break;
			case 'f':
				n_inter_frames = atoi(optarg);
				break;
			case 's':
				sleep_time_us = atoi(optarg);
				break;
			case 'd':
				data_length = atoi(optarg);
				break;
			case 'u':
				buffer_size = atoi(optarg);
				break;
			case 'o':
				dot_filepath = std::string(optarg);
				break;
			case 'c':
				no_copy_mode = false;
				break;
			case 'p':
				print_stats = true;
				break;
			case 'b':
				step_by_step = true;
				break;
			case 'g':
				debug = true;
				break;
			case 'w':
				active_waiting = true;
				break;
			case 'q':
				force_sequence = true;
				break;
			case 'h':
				std::cout << "usage: " << argv[0] << " [options]" << std::endl;
				std::cout << std::endl;
				std::cout << "  -t, --n-threads       "
				          << "Number of threads to run in parallel                                  "
				          << "[" << n_threads << "]" << std::endl;
				std::cout << "  -f, --n-inter-frames  "
				          << "Number of frames to process in one task                               "
				          << "[" << n_inter_frames << "]" << std::endl;
				std::cout << "  -s, --sleep-time      "
				          << "Sleep time duration in one task (microseconds)                        "
				          << "[" << sleep_time_us << "]" << std::endl;
				std::cout << "  -d, --data-length     "
				          << "Size of data to process in one task (in bytes)                        "
				          << "[" << data_length << "]" << std::endl;
				std::cout << "  -u, --buffer-size     "
				          << "Size of the buffer between the different stages of the pipeline       "
				          << "[" << data_length << "]" << std::endl;
				std::cout << "  -o, --dot-filepath    "
				          << "Path to dot output file                                               "
				          << "[" << (dot_filepath.empty() ? "empty" : "\"" + dot_filepath + "\"") << "]" << std::endl;
				std::cout << "  -c, --copy-mode       "
				          << "Enable to copy data in sequence (performance will be reduced)         "
				          << "[" << (no_copy_mode ? "true" : "false") << "]" << std::endl;
				std::cout << "  -b, --step-by-step    "
				          << "Enable step-by-step sequence execution (performance will be reduced)  "
				          << "[" << (step_by_step ? "true" : "false") << "]" << std::endl;
				std::cout << "  -p, --print-stats     "
				          << "Enable to print per task statistics (performance will be reduced)     "
				          << "[" << (print_stats ? "true" : "false") << "]" << std::endl;
				std::cout << "  -g, --debug           "
				          << "Enable task debug mode (print socket data)                            "
				          << "[" << (debug ? "true" : "false") << "]" << std::endl;
				std::cout << "  -q, --force-sequence  "
				          << "Force sequence instead of pipeline                                    "
				          << "[" << (force_sequence ? "true" : "false") << "]" << std::endl;
				std::cout << "  -w, --active-waiting  "
				          << "Enable active waiting in the pipeline synchronizations                "
				          << "[" << (active_waiting ? "true" : "false") << "]" << std::endl;
				std::cout << "  -h, --help            "
				          << "This help                                                             "
				          << "[false]" << std::endl;
				exit(0);
				break;
			default:
				break;
		}
	}

	std::cout << "####################################" << std::endl;
	std::cout << "# Micro-benchmark: Complexe pipeline #" << std::endl;
	std::cout << "####################################" << std::endl;
	std::cout << "#" << std::endl;

	std::cout << "# Command line arguments:" << std::endl;
	std::cout << "#   - n_threads      = " << n_threads << std::endl;
	std::cout << "#   - n_inter_frames = " << n_inter_frames << std::endl;
	std::cout << "#   - sleep_time_us  = " << sleep_time_us << std::endl;
	std::cout << "#   - data_length    = " << data_length << std::endl;
	std::cout << "#   - buffer_size    = " << buffer_size << std::endl;
	std::cout << "#   - dot_filepath   = " << (dot_filepath.empty() ? "[empty]" : dot_filepath.c_str()) << std::endl;
	std::cout << "#   - no_copy_mode   = " << (no_copy_mode ? "true" : "false") << std::endl;
	std::cout << "#   - print_stats    = " << (print_stats ? "true" : "false") << std::endl;
	std::cout << "#   - step_by_step   = " << (step_by_step ? "true" : "false") << std::endl;
	std::cout << "#   - debug          = " << (debug ? "true" : "false") << std::endl;
	std::cout << "#   - force_sequence = " << (force_sequence ? "true" : "false") << std::endl;
	std::cout << "#   - active_waiting = " << (active_waiting ? "true" : "false") << std::endl;
	std::cout << "#" << std::endl;

	if (!force_sequence && !no_copy_mode)
		std::clog << rang::tag::warning << "'no_copy_mode' has no effect with pipeline (it is always enable)" << std::endl;
	if (!force_sequence && step_by_step)
		std::clog << rang::tag::warning << "'step_by_step' is not available with pipeline" << std::endl;

	// modules creation
	const bool auto_reset = false;
	// modules creation
	module::Initializer<uint8_t> initializer(data_length);
	module::Finalizer  <uint8_t> finalizer  (data_length);

	std::vector<std::shared_ptr<module::Incrementer_io<uint8_t>>> incs_io(5);
	for (size_t s = 0; s < incs_io.size(); s++)
	{
		incs_io[s].reset(new module::Incrementer_io<uint8_t>(data_length));
		incs_io[s]->set_ns(sleep_time_us * 1000);
		incs_io[s]->set_custom_name("Inc_io" + std::to_string(s));
	}

    std::vector<std::shared_ptr<module::Incrementer<uint8_t>>> incs(5);
	for (size_t s = 0; s < incs.size(); s++)
	{
		incs[s].reset(new module::Incrementer<uint8_t>(data_length));
		incs[s]->set_ns(sleep_time_us * 1000);
		incs[s]->set_custom_name("Inc_io" + std::to_string(s));
	}

	/****************************************************************************************************************************/
	// Création d'un nouveau module Stateless
	module::Stateless multi_comp;
	multi_comp.set_name("comparator");
	auto& task_multi_comp = multi_comp.create_task("compare");
	auto sock_0 = multi_comp.create_socket_inout<uint8_t>(task_multi_comp,"inout_0",data_length);
	auto sock_1 = multi_comp.create_socket_inout<uint8_t>(task_multi_comp,"inout_1",data_length);
	
	multi_comp.create_codelet(task_multi_comp,[sock_0, sock_1,data_length](module::Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &mul_com = static_cast<module::Stateless&>(m);
		auto tab_0 = static_cast<uint8_t*>(t[sock_0].get_dataptr()); // Récupère le tableau de base qu'on doit multiplier
		auto tab_1 = (uint8_t*)(t[sock_1].get_dataptr()); // Le tableau après n addition
		// La comparaison des valeurs 
		for (size_t i=0;i<data_length;i++)
			if(tab_0[i] != tab_1[i]){
				std::cout << "Les valuers sont " << " Tab_0 : " << unsigned (tab_0[i]) <<  " Tab_1 : " << unsigned (tab_1[i]) << std::endl;
			}
		
		return runtime::status_t::SUCCESS;
	});
	/****************************************************************************************************************************/

	// sockets binding pour les incrémentations
	(*incs_io[0])[module::inc_io::sck::increment_io::inout] = initializer[module::ini::sck::initialize::out];
    (*incs[0])[module::inc::sck::increment::in] = initializer[module::ini::sck::initialize::out];
		for (size_t s = 0; s < incs_io.size() -1; s++)
        {
			(*incs_io[s+1])[module::inc_io::sck::increment_io::inout] = (*incs_io[s])[module::inc_io::sck::increment_io::inout];
            (*incs[s+1])[module::inc::sck::increment::in] = (*incs[s])[module::inc::sck::increment::out];
        }
	
	multi_comp["compare::inout_0"] = (*incs_io[incs_io.size()-1])[module::inc_io::sck::increment_io::inout];
	multi_comp["compare::inout_1"] = (*incs[incs.size()-1])[module::inc::sck::increment::out];
    
	finalizer[module::fin::sck::finalize::in] = multi_comp["compare::inout_1"];	
	
	/*###############################################################################################################################*/
														// Partie Pipeline 
 
	
	std::unique_ptr<runtime::Pipeline> pipeline_chain;

	
		pipeline_chain.reset(new runtime::Pipeline(
		                     initializer[module::ini::tsk::initialize], // first task of the sequence
		                     { // pipeline stage 0
		                       { { &initializer[module::ini::tsk::initialize] },   // first tasks of stage 0
		                         { &initializer[module::ini::tsk::initialize] } }, // last  tasks of stage 0
		                       // pipeline stage 1
		                       { { &(*incs_io[0])[module::inc_io::tsk::increment_io],&(*incs[0])[module::inc::tsk::increment] },   // first tasks of stage 1
		                         { &(*incs_io[incs_io.size()-1])[module::inc_io::tsk::increment_io], &(*incs[incs.size()-1])[module::inc::tsk::increment]} }, // last  tasks of stage 1

								{ { &task_multi_comp },   // first tasks of stage 1
		                         { &task_multi_comp} }, // last  tasks of stage 1
		                       // pipeline stage 2
		                       { {& finalizer[module::fin::tsk::finalize] },   // first tasks of stage 2
		                         {                                     } }, // last  tasks of stage 2
		                     },
		                     {
		                       1,                         // number of threads in the stage 0
		                       14,						// number of threads in the stage 1
							   1,
		                       1                          // number of threads in the stage 2
		                     },
		                     {
		                       buffer_size, // synchronization buffer size between stages 0 and 1
		                       buffer_size, // synchronization buffer size between stages 1 and 2
							   buffer_size, // synchronization buffer size between stages 1 and 2
		                     },
		                     {
		                       active_waiting, // type of waiting between stages 0 and 1 (true = active, false = passive)
		                       active_waiting, // type of waiting between stages 1 and 2 (true = active, false = passive)
							   active_waiting, // type of waiting between stages 1 and 2 (true = active, false = passive)
		                     }));
		pipeline_chain->set_n_frames(n_inter_frames);

	// Remplissage des donénes en entrée 
	auto tid = 0;
	for (auto cur_initializer : pipeline_chain.get()->get_stages()[0]->get_cloned_modules<module::Initializer<uint8_t>>(initializer))
	{
		std::vector<std::vector<uint8_t>> init_data(n_inter_frames, std::vector<uint8_t>(data_length, 0));
		for (size_t f = 0; f < n_inter_frames; f++)
			std::fill(init_data[f].begin(), init_data[f].end(), 1);
		cur_initializer->set_init_data(init_data);
		tid++;
			// Print les donées en entrées du vecteur 
	}



	if (!dot_filepath.empty())
	{
		std::ofstream file(dot_filepath);
		pipeline_chain->export_dot(file);
	}
	// configuration of the sequence tasks
	for (auto& mod : pipeline_chain->get_modules<module::Module>(false)) for (auto& tsk : mod->tasks)
	{
		tsk->reset          (           );
		tsk->set_debug      (true      ); // disable the debug mode
		tsk->set_debug_limit(16         ); // display only the 16 first bits if the debug mode is enabled
		tsk->set_stats      (print_stats); // enable the statistics
		tsk->set_fast       (true       ); // enable the fast mode (= disable the useless verifs in the tasks)
	}
	
	auto t_start = std::chrono::steady_clock::now();
	pipeline_chain->exec([](){return true;});
	std::chrono::nanoseconds duration = std::chrono::steady_clock::now() - t_start;
	auto elapsed_time = duration.count() / 1000.f / 1000.f;
	std::cout << "Sequence elapsed time: " << elapsed_time << " ms" << std::endl;

	// verification of the sequence execution
	bool tests_passed = true;
	tid = 0;
	
	for (auto cur_finalizer : pipeline_chain.get()->get_stages()[pipeline_chain.get()->get_stages().size()-1]->get_cloned_modules<module::Finalizer<uint8_t>>(finalizer))
	{
		for (size_t f = 0; f < n_inter_frames; f++)
		{
			const auto &final_data = cur_finalizer->get_final_data()[f];
			for (size_t d = 0; d < final_data.size(); d++)
			{

				std::cout <<"La donnée : " << d << " vaut : " << +final_data[d] << std::endl;

				auto expected = (int)(incs_io.size()+incs.size() + 1);
				expected = expected % 256;
				if (final_data[d] != expected)
				{
					std::cout << "# expected = " << +expected << " - obtained = "
					          << +final_data[d] << " (d = " << d << ", tid = " << tid << ")" << std::endl;
					tests_passed = false;
				}
			}
		}
		tid++;
	}
	
	if (tests_passed)
		std::cout << "# " << rang::style::bold << rang::fg::green << "Tests passed!" << rang::style::reset << std::endl;
	else
		std::cout << "# " << rang::style::bold << rang::fg::red << "Tests failed :-(" << rang::style::reset << std::endl;

	unsigned int test_results = !tests_passed;

	{
		pipeline_chain->set_n_frames(1);
		pipeline_chain->unbind_adaptors();
	}

	
	(*incs[0])[module::inc::sck::increment::in].unbind(initializer[module::ini::sck::initialize::out]);
	(*incs_io[0])[module::inc_io::sck::increment_io::inout].unbind(initializer[module::ini::sck::initialize::out]);

	for (size_t s = 0; s < incs_io.size() -1; s++)
    {
		(*incs_io[s+1])[module::inc_io::sck::increment_io::inout].unbind((*incs_io[s])[module::inc_io::sck::increment_io::inout]);
        (*incs[s+1])[module::inc::sck::increment::in].unbind((*incs[s])[module::inc::sck::increment::out]);
    }

	multi_comp["compare::inout_0"].unbind((*incs_io[incs_io.size()-1])[module::inc_io::sck::increment_io::inout]);
    multi_comp["compare::inout_1"].unbind((*incs[incs.size()-1])[module::inc::sck::increment::out]);

	finalizer[module::fin::sck::finalize::in].unbind(multi_comp["compare::inout_1"]);
	
	return test_results;

	
}
