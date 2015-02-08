//
//  sumator.cpp
//  
//
//  Created by Korin Wong-Horiuchi on 2/7/15.
//
//

/* sumator.cpp */

#include "sumator.h"
#include "object_type_db.h"

void Sumator::add(int value) {
    
    count+=value;

}

void Sumator::reset() {
    
    count=0;
}

int Sumator::get_total() const {
    
    return count;
}

void Sumator::_bind_methods() {
    
    ObjectTypeDB::bind_method("add",&Sumator::add);
    ObjectTypeDB::bind_method("reset",&Sumator::reset);
    ObjectTypeDB::bind_method("get_total",&Sumator::get_total);
}

Sumator::Sumator() {
    count=0;
}