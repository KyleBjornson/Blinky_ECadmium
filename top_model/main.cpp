#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupled_model.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>


#include "../vendor/NDTime.hpp"
#include "../vendor/iestream.hpp"

#include "../data_structures/message.hpp"

#include "../atomics/digitalOutput.hpp"
#include "../atomics/blinky.hpp"

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;

#ifdef ECADMIUM
// You must increase stack size for ECADMIUM. 
// The main functionality will be ran in a new thread with increased stack size
// See below for reference:
// https://os.mbed.com/questions/79584/Change-main-thread-stack-size/
Thread app_thread(osPriorityNormal, 16*1024); // 16k stack
void run_app();
#endif

int main(int argc, char ** argv) {
  #ifdef ECADMIUM
  app_thread.start(&run_app);
  // Let the main thread die on the embedded platform. 
}
// run_app is only used for embedded threading, everything runs in main when simulated.
void run_app(){
#endif

  // all simulation timing and I/O streams are ommited when running embedded 
  #ifndef ECADMIUM
    auto start = hclock::now(); //to measure simulation execution time

/*************** Loggers *******************/

  static std::ofstream out_data("blinky_test_output.txt");
  #endif

  struct oss_sink_provider{
      static std::ostream& sink(){   
          #ifdef ECADMIUM
            return cout;
          #else       
            return out_data;
          #endif
      }
  };

using info=cadmium::logger::logger<cadmium::logger::logger_info, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using debug=cadmium::logger::logger<cadmium::logger::logger_debug, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using state=cadmium::logger::logger<cadmium::logger::logger_state, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using log_messages=cadmium::logger::logger<cadmium::logger::logger_messages, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using routing=cadmium::logger::logger<cadmium::logger::logger_message_routing, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using global_time=cadmium::logger::logger<cadmium::logger::logger_global_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using local_time=cadmium::logger::logger<cadmium::logger::logger_local_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
using log_all=cadmium::logger::multilogger<info, debug, state, log_messages, routing, global_time, local_time>;

using logger_top=cadmium::logger::multilogger<log_messages, global_time>;


/*******************************************/



/********************************************/
/****** APPLICATION GENERATOR ****************/
/********************************************/
using AtomicModelPtr=std::shared_ptr<cadmium::dynamic::modeling::model>;
using CoupledModelPtr=std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>>;

/********************************************/
/****** blinky *******************/
/********************************************/

AtomicModelPtr blinky1 = cadmium::dynamic::translate::make_dynamic_atomic_model<Blinky, TIME>("blinky1");

/********************************************/
/****** DigitalOutput1 *******************/
/********************************************/

AtomicModelPtr digitalOutput1 = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>();

/************************/
/*******TOP MODEL********/
/************************/
cadmium::dynamic::modeling::Ports iports_TOP = {};
cadmium::dynamic::modeling::Ports oports_TOP = {};
cadmium::dynamic::modeling::Models submodels_TOP =  {blinky1, digitalOutput1};
cadmium::dynamic::modeling::EICs eics_TOP = {};
cadmium::dynamic::modeling::EOCs eocs_TOP = {};
cadmium::dynamic::modeling::ICs ics_TOP = {
   cadmium::dynamic::translate::make_IC<blinky_defs::dataOut, digitalOutput_defs::in>("blinky1","digitalOutput1")
};
CoupledModelPtr TOP = std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
 "TOP", 
 submodels_TOP, 
 iports_TOP, 
 oports_TOP, 
 eics_TOP, 
 eocs_TOP, 
 ics_TOP 
 );

///****************////
    #ifndef ECADMIUM
    auto elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Model Created. Elapsed time: " << elapsed1 << "sec" << endl;
    #endif

    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    #ifndef ECADMIUM
    elapsed1 = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Runner Created. Elapsed time: " << elapsed1 << "sec" << endl;

    cout << "Simulation starts" << endl;
    #endif

    r.run_until(NDTime("00:10:00:000"));
    #ifndef ECADMIUM
    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(hclock::now() - start).count();
    cout << "Simulation took:" << elapsed << "sec" << endl;
    return 0;
    #endif
}