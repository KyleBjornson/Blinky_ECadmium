/**
* or:
* Cadmium implementation of CD++ or atomic model
*/

#ifndef BOOST_SIMULATION_PDEVS_DIGITALOUTPUT_HPP
#define BOOST_SIMULATION_PDEVS_DIGITALOUTPUT_HPP

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
    struct digitalOutput_defs{
        struct in : public in_port<Message_t> {
        };
    };

    template<typename TIME>
    class DigitalOutput {
        using defs=digitalOutput_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            #ifdef ECADMIUM
            DigitalOut* digiPin;
            #endif
            
            // default c onstructor
            DigitalOutput() noexcept{
              state.output = false;
              #ifdef ECADMIUM
              digiPin = new DigitalOutput(D0);
              #endif
            }

            #ifdef ECADMIUM
            DigitalOutput(PinName pin) {
              state.output = false;
              digiPin = new DigitalOut(pin);
            }
            #endif
            
            // state definition
            struct state_type{
              bool output;
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
              digiPin->write(state.output ? 1 : 0);
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
              // Message_t out;              
              // out.value = (state.output ? 1 : 0);
              // get_messages<typename defs::dataOut>(bags).push_back(out);
    
              return bags;
            }

            // time_advance function
            TIME time_advance() const {     
              // #ifdef ECADMIUM
              //   return TIME("10:00:00");
              // #else
                return std::numeric_limits<TIME>::infinity();
              //#endif
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename DigitalOutput<TIME>::state_type& i) {
              os << "Pin Out: " << (i.output ? 1 : 0); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_DIGITALOUTPUT_HPP