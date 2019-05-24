/**
* or:
* Cadmium implementation of CD++ or atomic model
*/

#ifndef BOOST_SIMULATION_PDEVS_DIGITALINPUT_HPP
#define BOOST_SIMULATION_PDEVS_DIGITALINPUT_HPP

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
    struct digitalInput_defs{
        struct out : public out_port<Message_t> {
        };
    };

    template<typename TIME>
    class DigitalInput {
        using defs=digitalInput_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            #ifdef ECADMIUM
              DigitalIn* digiPin;
            #endif

            TIME   pollingRate;
            // default c onstructor
            DigitalInput() noexcept{
              pollingRate = TIME("00:00:00:100");
              state.output = false;
              #ifdef ECADMIUM
              digiPin = new DigitalIn(D0);
              #endif
            }

            #ifdef ECADMIUM
            DigitalInput(PinName pin) {
              pollingRate = TIME("00:00:01:00");
              digiPin = new DigitalIn(pin);
              state.output = digiPin->read();
            }
            #endif
            
            // state definition
            struct state_type{
              bool output;
            }; 
            state_type state;
            // ports definition

            using input_ports=std::tuple<>;
            using output_ports=std::tuple<typename defs::out>;

            // internal transition
            void internal_transition() {
              #ifdef ECADMIUM
              state.output = digiPin->read();
              #endif
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
            }
            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
                internal_transition();
                external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
               Message_t out;              
               out.value = (state.output ? 1 : 0);
               get_messages<typename defs::out>(bags).push_back(out);
    
              return bags;
            }

            // time_advance function
            TIME time_advance() const {     
                return pollingRate;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename DigitalInput<TIME>::state_type& i) {
              os << "Input Pin: " << (i.output ? 1 : 0); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_DIGITALINPUT_HPP