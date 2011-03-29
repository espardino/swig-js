/* -----------------------------------------------------------
 * jsv8kw.swg
 * ------------------------------------------------------------ */

#ifndef JSV8_JSKW_SWG_
#define JSV8_JSKW_SWG_

/* Warnings for javascript keywords */
#define JSKW(x) %keywordwarn("'" `x` "' is a javascript keyword, renaming to 'C_" `x` "'",rename="C_%s",fullname=1)  `x`

//TODO: add all javascript keywords
JSKW(function);
JSKW(if);
JSKW(else);
JSKW(end);

#undef JSKW

#endif //JSV8_JSKW_SWG_
