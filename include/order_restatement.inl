#if !defined(FIELD) || !defined(VAR_FIELD) || !defined(CHAR_FIELD)
#error You need to define FIELD, VAR_FIELD and CHAR_FIELD macro
#else

VAR_FIELD(cl_ord_id, text, 18, 20)
CHAR_FIELD(reason, 46)
FIELD(active_volume, double, 55, 4)
VAR_FIELD(secondary_order_id, base36, 59, 8)

#undef FIELD
#undef VAR_FIELD
#undef CHAR_FIELD

#endif
