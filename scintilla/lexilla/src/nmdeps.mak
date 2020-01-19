# Created by DepGen.py. To recreate, run 'python DepGen.py'.
$(DIR_O)/Lexilla.obj: \
	../../lexilla/src/Lexilla.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../lexlib/LexerModule.h \
	../../lexlib/CatalogueModules.h
$(DIR_O)/Accessor.obj: \
	../../lexlib/Accessor.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h
$(DIR_O)/CharacterCategory.obj: \
	../../lexlib/CharacterCategory.cxx \
	../../lexlib/CharacterCategory.h
$(DIR_O)/CharacterSet.obj: \
	../../lexlib/CharacterSet.cxx \
	../../lexlib/CharacterSet.h
$(DIR_O)/DefaultLexer.obj: \
	../../lexlib/DefaultLexer.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/LexerModule.h \
	../../lexlib/DefaultLexer.h
$(DIR_O)/LexerBase.obj: \
	../../lexlib/LexerBase.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/LexerModule.h \
	../../lexlib/LexerBase.h
$(DIR_O)/LexerModule.obj: \
	../../lexlib/LexerModule.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/LexerModule.h \
	../../lexlib/LexerBase.h \
	../../lexlib/LexerSimple.h
$(DIR_O)/LexerNoExceptions.obj: \
	../../lexlib/LexerNoExceptions.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/LexerModule.h \
	../../lexlib/LexerBase.h \
	../../lexlib/LexerNoExceptions.h
$(DIR_O)/LexerSimple.obj: \
	../../lexlib/LexerSimple.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/PropSetSimple.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/LexerModule.h \
	../../lexlib/LexerBase.h \
	../../lexlib/LexerSimple.h
$(DIR_O)/PropSetSimple.obj: \
	../../lexlib/PropSetSimple.cxx \
	../../lexlib/PropSetSimple.h
$(DIR_O)/StyleContext.obj: \
	../../lexlib/StyleContext.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/StyleContext.h \
	../../lexlib/CharacterSet.h
$(DIR_O)/WordList.obj: \
	../../lexlib/WordList.cxx \
	../../lexlib/WordList.h
$(DIR_O)/LexCPP.obj: \
	../../lexers/LexCPP.cxx \
	../../include/ILexer.h \
	../../include/Sci_Position.h \
	../../include/Scintilla.h \
	../../include/SciLexer.h \
	../../lexlib/StringCopy.h \
	../../lexlib/WordList.h \
	../../lexlib/LexAccessor.h \
	../../lexlib/Accessor.h \
	../../lexlib/StyleContext.h \
	../../lexlib/CharacterSet.h \
	../../lexlib/LexerModule.h \
	../../lexlib/OptionSet.h \
	../../lexlib/SparseState.h \
	../../lexlib/SubStyles.h
