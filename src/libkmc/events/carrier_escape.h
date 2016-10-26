/*
 * Copyright 2009-2013 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __VOTCA_KMC_CarrierEscape_H
#define __VOTCA_KMC_CarrierEscape_H

#include <votca/kmc/event.h>
#include <votca/kmc/carrier.h>

namespace votca { namespace kmc {
    
class CarrierEscape : public Event {
    
public:

    std::string Type(){ return "carrier escape"; } ;
    
    // electron to move
    Carrier* carrier;
    
    void Initialize( Carrier* _carrier) {
        carrier = _carrier;
        Enable();
    }
    
    // changes to be made after this event occurs
    virtual void OnExecute(  State* state, votca::tools::Random2 *RandomVariable ) {
        
        double cumulative_rate = CumulativeRate();
        double u = 1.0 - RandomVariable->rand_uniform();
        //std::cout << "[" << u  << "]" << std::endl;
        // find a subordinate event to execute
        Event* subordinate;
        for( Event::iterator it_subordinate = begin(); it_subordinate != end() ; ++it_subordinate ) {
            subordinate = (*it_subordinate);
            u -= subordinate->CumulativeRate()/cumulative_rate;
            if(u <= 0) break;
        }

        subordinate->OnExecute( state, RandomVariable );
             
    };
    
private:



};


}}
#endif 
