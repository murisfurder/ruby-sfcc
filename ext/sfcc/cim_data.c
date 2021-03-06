
#include "cim_data.h"
#include "cim_type.h"

static void
dealloc(CIMCData *data)
{
/*  fprintf(stderr, "Sfcc_dealloc_data %p\n", data); */
  free(data);
}

/**
 * call-seq:
 *   state()
 *
 * Get the state of the data
 */
static VALUE state(VALUE self)
{
  CIMCData *data;
  Data_Get_Struct(self, CIMCData, data);
  return UINT2NUM(data->state);
}

/**
 * call-seq:
 *   type() -> Cim::Type
 *
 * Get the type of the data
 */
static VALUE type(VALUE self)
{
  CIMCData *data;
  Data_Get_Struct(self, CIMCData, data);
  return Sfcc_wrap_cim_type(data->type);
}

/**
 * call-seq:
 *   value()
 *
 * Get the value of the data
 */
static VALUE value(VALUE self)
{
  CIMCData *data;
  Data_Get_Struct(self, CIMCData, data);
  return sfcc_cimdata_to_value(data, Qnil);
}


/**
 * call-seq:
 *   state_is(state) -> Boolean
 *
 * Check the value state
 */
static VALUE state_is(VALUE self, VALUE state)
{
  CIMCData *data;
  Data_Get_Struct(self, CIMCData, data);
  if (data->state & FIX2INT(state))
    return Qtrue;
  return Qfalse;
}


VALUE
Sfcc_wrap_cim_data(CIMCData *data)
{
  CIMCData *ptr = (CIMCData *)malloc(sizeof(CIMCData));
  if (!ptr)
    rb_raise(rb_eNoMemError, "Cannot alloc rb_sfcc_data");
  memcpy(ptr, data, sizeof(CIMCData));
  return Data_Wrap_Struct(cSfccCimData, NULL, dealloc, ptr);
}


VALUE cSfccCimData;
void init_cim_data()
{
  VALUE sfcc = rb_define_module("Sfcc");
  VALUE cimc = rb_define_module_under(sfcc, "Cim");

  /**
   * data on the CIM namespace
   */
  VALUE klass = rb_define_class_under(cimc, "Data", rb_cObject);
  cSfccCimData = klass;

  rb_define_method(klass, "state", state, 0);
  rb_define_method(klass, "type", type, 0);
  rb_define_method(klass, "value", value, 0);

  /* Value state */
  rb_define_method(klass, "state_is", state_is, 1);

  rb_define_const(klass, "Good", INT2FIX(CIMC_goodValue)); /* (0) */
  rb_define_const(klass, "Null", INT2FIX(CIMC_nullValue)); /* (1<<8) */
  rb_define_const(klass, "Key", INT2FIX(CIMC_keyValue)); /*  (2<<8) */
  rb_define_const(klass, "NotFound", INT2FIX(CIMC_notFound)); /*  (4<<8) */
  rb_define_const(klass, "Bad", INT2FIX(CIMC_badValue)); /*  (0x80<<8) */
}
