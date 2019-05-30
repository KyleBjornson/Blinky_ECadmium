/**
* Ben Earle and Kyle Bjornson
* ARSLab - Carleton University
*
* Blinky:
* Simple modle to toggle the LED using DEVS internal transitions.
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

using namespace cadmium;
using namespace std;

//Port definition
    struct blinky_defs {
        struct dataOut : public out_port<bool> { };
        struct in : public in_port<bool> { };
    };

    template<typename TIME>
    class Blinky {
        using defs=blinky_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME   slowToggleTime;
            TIME   fastToggleTime;
            // default constructor
            Blinky() noexcept{
              slowToggleTime  = TIME("00:00:00:750");
              fastToggleTime  = TIME("00:00:00:250");
              state.lightOn = false;
              state.fastToggle = false;
            }
            
            // state definition
            struct state_type{
              bool lightOn;
              bool fastToggle;
            }; 
            state_type state;
            // ports definition

            using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<typename defs::dataOut>;

            // internal transition
            void internal_transition() {
              state.lightOn=!state.lightOn;
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
              for(const auto &x : get_messages<typename defs::in>(mbs)){
                // if(x.value == 0)
                //   state.fastToggle = !state.fastToggle;
                state.fastToggle = (x == 0);
              }
            }
            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
              internal_transition();
              external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
              bool out;              
              out = (state.lightOn ? 1 : 0);
              get_messages<typename defs::dataOut>(bags).push_back(out);
                
              return bags;
            }

            // time_advance function
            TIME time_advance() const {  
              if(state.fastToggle)
                return fastToggleTime;
              else 
                return slowToggleTime;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename Blinky<TIME>::state_type& i) {
              os << "Output: " << (i.lightOn ? 1 : 0); 
              return os;
            }
        };     


#endif // BOOST_SIMULATION_PDEVS_BLINKY_HPP