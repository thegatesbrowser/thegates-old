#ifndef VARIANT_TOOLS_H
#define VARIANT_TOOLS_H

#include "core/variant/variant.h"
#include "core/variant/variant_parser.h"

static inline String var_to_str(const Variant &p_var) {
	String vars;
	VariantWriter::write_to_string(p_var, vars);
	return vars;
}

static inline Variant str_to_var(const String &p_var) {
	VariantParser::StreamString ss;
	ss.s = p_var;

	String errs;
	int line;
	Variant ret;
	(void)VariantParser::parse(&ss, ret, errs, line);

	return ret;
}

#endif // VARIANT_TOOLS_H
