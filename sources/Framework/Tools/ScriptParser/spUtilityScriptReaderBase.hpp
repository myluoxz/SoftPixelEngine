/*
 * Script base header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_UTILITY_SCRIPT_READER_BASE_H__
#define __SP_UTILITY_SCRIPT_READER_BASE_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_TOKENPARSER


#include "Framework/Tools/ScriptParser/spUtilityTokenParser.hpp"


namespace sp
{
namespace tool
{


/**
Script reader base class.
\since Version 3.3
*/
class SP_EXPORT ScriptReaderBase
{
    
    public:
        
        virtual ~ScriptReaderBase();
        
    protected:
        
        /* === Functions === */
        
        ScriptReaderBase();
        
        bool exitWithError(const io::stringc &Message, const SToken* InvalidToken);
        bool exitWithError(const io::stringc &Message, bool AppendTokenPos = true);
        
        bool validateBrackets();
        
        bool nextToken(bool IgnoreWhiteSpaces = true);
        bool nextToken(const ETokenTypes NextTokenType);
        bool nextTokenCheck(const ETokenTypes CheckTokenType);
        
        void push(bool UsePrevIndex = true);
        void pop(bool UsePrevIndex = true);
        
        /* === Inline functions === */
        
        inline ETokenTypes type() const
        {
            return Tkn_->Type;
        }
        
        /* === Members === */
        
        TokenParser Parser_;
        TokenIteratorPtr TokenIt_;
        SToken* Tkn_;
        
};


} // /namespace tool

} // /namespace sp


#endif

#endif



// ================================================================================