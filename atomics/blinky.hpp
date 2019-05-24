/**
* or:
* Cadmium implementation of CD++ or atomic model
*/

#ifndef BOOST_SIMULATION_PDEVS_BLINKY_HPP
#define BOOST_SIMULATION_PDEVS_BLINKY_HPP

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

using namespace cadmium;
using namespace std;

//Port definition
    struct blinky_defs {
        struct dataOut : public out_port<Message_t> { };
    };

    template<typename TIME>
    class Blinky {
        using defs=blinky_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME   lightToggleTime;
            // default constructor
            Blinky() noexcept{
              lightToggleTime  = TIME("00:00:10");
              state.lightOn = false;
            }
            
            // state definition
            struct state_type{
              bool lightOn;
            }; 
            state_type state;
            // ports definition

            using input_ports=std::tuple<>;
            using output_ports=std::tuple<typename defs::dataOut>;

            // internal transition
            void internal_transition() {
              state.lightOn=!state.lightOn;
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
              // There are no inputs, therefore there should never be an external transition.
              assert(false);
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
              out.value = (state.lightOn ? 1 : 0);
              get_messages<typename defs::dataOut>(bags).push_back(out);
                
              return bags;
            }

            // time_advance function
            TIME time_advance() const {  
              return lightToggleTime;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename Blinky<TIME>::state_type& i) {
              os << "Output: " << (i.lightOn ? 1 : 0); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_BLINKY_HPP