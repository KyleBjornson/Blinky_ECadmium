/**
* Kyle Bjornson
* ARSLab - Carleton University
*
* MultiplyCadmium:
* Toy example
*/

#ifndef SIMULATION_PDEVS_MULTIPLY_HPP
#define SIMULATION_PDEVS_MULTIPLY_HPP


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
    struct Multiply_defs{
        struct out : public out_port<Message_t> {
        };
        struct in : public in_port<Message_t> {
        };
    };
   
    template<typename TIME>
    class Multiply{
        using defs=Multiply_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME   preparationTime;
            // default constructor
            Multiply() noexcept{
              preparationTime  = TIME("00:00:05");
              state.val = 0;
            }
            
            // state definition
            struct state_type{
              int val;
            }; 
            state_type state;
            // ports definition
            using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<typename defs::out>;

            // internal transition
            void internal_transition() {
              state.val = 0;
            }

            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
              if(get_messages<typename defs::in>(mbs).size()>1) assert(false && "one message per time uniti");
              for(const auto &x : get_messages<typename defs::in>(mbs)){
                state.val = static_cast < int > (x.value);
                state.val *= 2;
                //state.sending = true;
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
              Message_t out;              
              out.value = state.val;
              get_messages<typename defs::out>(bags).push_back(out);
                
              return bags;

            }

            // time_advance function
            TIME time_advance() const {  
              TIME next_internal;
              if (state.val != 0) {
                next_internal = preparationTime;
              }else {
                next_internal = std::numeric_limits<TIME>::infinity();
              }    
              return next_internal;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename Multiply<TIME>::state_type& i) {
                os << "val: " << i.val; 
            return os;
            }
        };     
  

#endif // SIMULATION_PDEVS_MULTIPLY_HPP