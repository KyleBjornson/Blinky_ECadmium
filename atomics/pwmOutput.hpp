/**
* THIS CODE IS UNTESTED - THE BOARD USED AT ARSLABS DOES NOT HAVE A DAC
*/

#ifndef BOOST_SIMULATION_PDEVS_PWMOUTPUT_HPP
#define BOOST_SIMULATION_PDEVS_PWMOUTPUT_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

#include "../data_structures/message.hpp"

#ifdef ECADMIUM
#include "../mbed.h"
#endif

using namespace cadmium;
using namespace std;

//Port definition
    struct pwmOutput_defs{
        struct in : public in_port<Message_t> {
        };
    };

    template<typename TIME>
    class PwmOutput {
        using defs=pwmOutput_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            #ifdef ECADMIUM
            PwmOut* pwmPin;
            #endif
            
            // default c onstructor
            PwmOutput() noexcept{
              state.output = 0;
              #ifdef ECADMIUM
              pwmPin = new mbed::PwmOut(PWM_OUT);
              #endif
            }

            #ifdef ECADMIUM
            PwmOutput(PinName pin) {
              state.output = 0;
              pwmPin = new mbed::PwmOut(pin);
              pwmPin->period_ms(10);
              pwmPin->pulsewidth_ms(0);
            }
            #endif
            
            // state definition
            struct state_type{
              float output;
            }; 
            state_type state;
            // ports definition

            using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<>;

            // internal transition
            void internal_transition() {
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
              for(const auto &x : get_messages<typename defs::in>(mbs)){
                state.output = x.value;
              }
              #ifdef ECADMIUM
              pwmPin->write(state.output);
              #endif
            }
            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
              return bags;
            }

            // time_advance function
            TIME time_advance() const {     
                return std::numeric_limits<TIME>::infinity();
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename PwmOutput<TIME>::state_type& i) {
              os << "Pin Out: " << (i.output ? 1 : 0); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_PWMOUTPUT_HPP