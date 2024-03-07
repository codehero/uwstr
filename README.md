# uwstr: unobtrusive C string library #

Description
---------

This is a work in progress!

The uwstr library provides the following functionality
 * Safe, bounds checked string operations
 * Propagated errors instead of overflowed buffers, truncated results or crashes
 * Uses char pointers as the string datatype (no special struct)
 * Easy to use but rigorously defined

Propagated Errors
----------

The numerical computing community has long benefitted from error propagation in floating point calculations, made possible by the NaN value in IEEE 754.
NaN values poison the end result of the calculation without introducing buffer overflows or other critical vulnerabilities.
This approach eliminates the need for extensive error checking on each calculation.

Typically, string processing code typically omits such pedantic rigor, and thus would benefit from an adaptation of NaN concepts.
In a similar vein, uwstr defines the notion of the wynnstring (symbolized by ᚹ in formal descriptions).
The wynnstring is the representation of a string derived from invalidly formatted UTF-X or the result of a string operation with bad input.


Semantics
---------

Languages:
 * uint8_t : (values 0-255, represented here in hexadecimal)
   * Σ = { 0x00, 0x01, 0x02, ..., 0xFC, 0xFD, 0xFE, 0xFF }
	 * L = Σ*
 * UTF-8 : Σ_A ∪ Σ_A

Term definitions:
 * codepoint: A valid Unicode value
 * character: A combination of 1 or more codepoints
 * code unit: A representation of a single, possibly partial, codepoint
 * string: A conceptual sequence of 0 or more symbols
 * cstring: A sequence of 0 or more bytes terminated by a NUL character (byte value 0x00)
 * wynnchar: The character in a cstring that invalidates the UTF representation.
 * wynnstring: A string of unspecified length

Defines
 * NULL: Signifies the absence of a string.
 * ε: Signifies the empty or length 0 zero string. In other words, a string where the first codepoint is the null terminator.
 * ƿ: Signifies the wynnstring

Conformance Levels
----------

 * "Conformance Levels" defining increasing levels of functionality
 * Extensible to UTF-16 and UTF-32 support
 * Extensible to wchar_t, char16_t and char32_t support
 * Adherence to Unicode standard code points




https://en.wikipedia.org/wiki/Wynn
https://en.wikipedia.org/wiki/Character_encoding#Terminology

http://locklessinc.com/articles/dynamic_cstrings/
https://github.com/antirez/sds
https://github.com/lemire/fastvalidate-utf-8
http://bstring.sourceforge.net/
http://www.and.org/ustr/
https://www.johndcook.com/blog/cplusplus_strings/
