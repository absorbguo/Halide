#include "IROperator.h"

namespace Halide { 
namespace Internal {

bool is_const(Expr e) {
    if (e.as<IntImm>()) return true;
    if (e.as<FloatImm>()) return true;
    if (const Ramp *r = e.as<Ramp>()) {
        return is_const(r->base) && is_const(r->stride);
    }

    if (const Broadcast *b = e.as<Broadcast>()) {
        return is_const(b->value);
    }
    return false;
            
}

bool is_positive_const(Expr e) {
    if (const IntImm *i = e.as<IntImm>()) return i->value > 0;
    if (const FloatImm *f = e.as<FloatImm>()) return f->value > 0.0f;
    if (const Ramp *r = e.as<Ramp>()) {
        // slightly conservative
        return is_positive_const(r->base) && is_positive_const(r->stride);
    }
    if (const Broadcast *b = e.as<Broadcast>()) {
        return is_positive_const(b->value);
    }
    return false;
}

bool is_negative_const(Expr e) {
    if (const IntImm *i = e.as<IntImm>()) return i->value < 0;
    if (const FloatImm *f = e.as<FloatImm>()) return f->value < 0.0f;
    if (const Ramp *r = e.as<Ramp>()) {
        // slightly conservative
        return is_negative_const(r->base) && is_negative_const(r->stride);
    }
    if (const Broadcast *b = e.as<Broadcast>()) {
        return is_negative_const(b->value);
    }
    return false;
}

bool is_zero(Expr e) {
    if (const IntImm *int_imm = e.as<IntImm>()) return int_imm->value == 0;
    if (const FloatImm *float_imm = e.as<FloatImm>()) return float_imm->value == 0;
    if (const Cast *c = e.as<Cast>()) return is_zero(c->value);
    if (const Broadcast *b = e.as<Broadcast>()) return is_zero(b->value);
    return false;
}
        
bool is_one(Expr e) {
    if (const IntImm *int_imm = e.as<IntImm>()) return int_imm->value == 1;
    if (const FloatImm *float_imm = e.as<FloatImm>()) return float_imm->value == 1.0f;
    if (const Cast *c = e.as<Cast>()) return is_one(c->value);
    if (const Broadcast *b = e.as<Broadcast>()) return is_one(b->value);
    return false;
}
        
Expr make_zero(Type t) {
    if (t == Int(32)) return 0;
    if (t == Float(32)) return 0.0f;
    if (t.is_vector()) {
        return new Broadcast(make_zero(t.element_of()), t.width);
    }
    return new Cast(t, 0);
}
        
Expr make_one(Type t) {
    if (t == Int(32)) return 1;
    if (t == Float(32)) return 1.0f;
    if (t.is_vector()) {
        return new Broadcast(make_one(t.element_of()), t.width);
    }
    return new Cast(t, 1);
}
    
Expr const_true(int w) {
    return make_one(UInt(1, w));
}

Expr const_false(int w) {
    return make_zero(UInt(1, w));
}
    
}
}
