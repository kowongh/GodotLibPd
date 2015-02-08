//
//  register_types.cpp
//  
//
//  Created by Korin Wong-Horiuchi on 2/7/15.
//
//

/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "sumator.h"

void register_sumator_types() {
    
    ObjectTypeDB::register_type<Sumator>();
    
}

void unregister_sumator_types() {
    //nothing to do here
}