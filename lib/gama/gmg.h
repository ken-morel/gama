/**
 * @file gmg.h
 * @brief Provides a generic macro for generating debug string representations of Gama types.
 *
 * This header defines a convenient macro `gmg` that dispatches to type-specific
 * formatting functions (e.g., `gmg_color`, `gmg_pos3`) to convert Gama data
 * structures into human-readable strings for debugging purposes.
 */
#ifndef GMG_H_INCLUDED
#define GMG_H_INCLUDED
/**
 * @def gmg(type, obj)
 * @brief Generic macro to get a string representation of a Gama type for debugging.
 *
 * This macro calls a type-specific formatting function (e.g., `gmg_color`,
 * `gmg_pos3`) to convert a Gama object into a string. The actual formatting
 * functions are typically defined in the respective type's header or in debug.h.
 *
 * @param type The base name of the type's formatting function (e.g., `color`, `pos3`).
 * @param obj The object instance to convert to a string.
 */
#define gmg(type, obj) gmg_##type(obj)

#endif // GMG_H_INCLUDED
