typedef union {
  char* str;
  MenuElem* mItem;
  AttrStream* aStream;
  unsigned int modifier;
} YYSTYPE;
#define	VARIABLE	257
#define	MENU	258
#define	SHORTCUT	259
#define	APP	260
#define	KEY	261
#define	IND	262
#define	EXITDLG	263
#define	STARTUP	264
#define	ACC	265
#define	TBSC	266
#define	VAR	267
#define	STRING	268
#define	FUNC	269
#define	PLUS	270
#define	MINUS	271


extern YYSTYPE yylval;
