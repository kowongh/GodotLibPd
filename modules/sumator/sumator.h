//
//  sumator.h
//  
//
//  Created by Korin Wong-Horiuchi on 2/7/15.
//
//

/* sumator.h */
#ifndef SUMATOR_H
#define SUMATOR_H

#include "reference.h"

class Sumator : public Reference {
    OBJ_TYPE(Sumator,Reference);
    
    int count;
    
protected:
    static void _bind_methods();
public:
    
    void add(int value);
    void reset();
    int get_total() const;
    
    Sumator();
};

#endif