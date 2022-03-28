#if !defined(FIELD) || !defined(VAR_FIELD) || !defined(CHAR_FIELD)
#error You need to define FIELD, VAR_FIELD and CHAR_FIELD macro
#else

VAR_FIELD(cl_ord_id, text, 18, 20)
VAR_FIELD(exec_id, base36, 38, 8)
FIELD(filled_volume, double, 46, 4)
FIELD(price, price, 50, 8)
FIELD(active_volume, double, 58, 4)
CHAR_FIELD(liquidity_indicator, 62)
VAR_FIELD(symbol, text, 78, 8)
VAR_FIELD(last_mkt, text, 86, 4)
VAR_FIELD(fee_code, text, 90, 2)

#undef FIELD
#undef VAR_FIELD
#undef CHAR_FIELD

#endif
